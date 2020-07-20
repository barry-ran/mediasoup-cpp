
#include "Worker.hpp"
#include "Logger.hpp"
#include "Process.hpp"
#include "Mediasoup.hpp"

namespace mediasoup
{

#define MEDIASOUP_VERSION "MEDIASOUP_VERSION="##__MEDIASOUP_VERSION__

// If env MEDIASOUP_WORKER_BIN is given, use it as worker binary.
// Otherwise if env MEDIASOUP_BUILDTYPE is 'Debug' use the Debug binary.
// Otherwise use the Release binary.
std::string Worker::s_workerBin = "";

std::vector<std::string> StringSplit(const std::string& s, const std::string& delim = ",")
{
	std::vector<std::string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) {
		return elems;
	}

	while (pos < len) {
		int find_pos = s.find(delim, pos);
		if (find_pos < 0) {
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}

void StaticOnAllocCB(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf) {
	((Worker*)(handle->data))->OnAllocCB(handle, suggestedSize, buf);
}

void StaticOnReadCB(uv_stream_t* stream, ssize_t nRead, const uv_buf_t* buf) {
	((Worker*)(stream->data))->OnReadCB(stream, nRead, buf);
}

Worker::Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings) {
	MS_lOGGERF();
	m_settings = workerSettings;

	getWorkBinPath();
	MS_lOGGERI("mediasoup-worker path:{}", s_workerBin);

	RegisterObserver(obs);
	
	NotifyObserver(std::bind(&IWorker::Observer::OnSuccess, std::placeholders::_1));
}

Worker::~Worker() {
	MS_lOGGERF();
	UnregisterObserver();
}

bool Worker::Init() {
	MS_lOGGERF();

	std::string spawnBin = s_workerBin;
	std::vector<std::string> spawnArgs;

	if ("true" == Process::GetInstance().Environ("MEDIASOUP_USE_VALGRIND")) {
		spawnBin = Process::GetInstance().Environ("MEDIASOUP_VALGRIND_BIN").empty() ?
			"valgrind" : Process::GetInstance().Environ("MEDIASOUP_VALGRIND_BIN");

		if (!Process::GetInstance().Environ("MEDIASOUP_VALGRIND_OPTIONS").empty()) {
			std::vector<std::string> options = StringSplit(Process::GetInstance().Environ("MEDIASOUP_VALGRIND_OPTIONS"), " ");
			std::move(options.begin(), options.end(), std::back_inserter(spawnArgs));			
		}
		spawnArgs.push_back(spawnBin);
	}

	if (!m_settings.logLevel.empty()) {
		spawnArgs.push_back("--logLevel=" + m_settings.logLevel);
	}
	
	for (const auto& tag : m_settings.logTags) {
		if (tag.empty()) {
			continue;
		}
		spawnArgs.push_back("--logTag=" + tag);
	}

	spawnArgs.push_back("--rtcMinPort=" + std::to_string(m_settings.rtcMinPort));
	spawnArgs.push_back("--rtcMaxPort=" + std::to_string(m_settings.rtcMaxPort));

	if (!m_settings.dtlsCertificateFile.empty()) {
		spawnArgs.push_back("--dtlsCertificateFile=" + m_settings.dtlsCertificateFile);
	}

	if (!m_settings.dtlsPrivateKeyFile.empty()) {
		spawnArgs.push_back("--dtlsPrivateKeyFile=" + m_settings.dtlsPrivateKeyFile);
	}

	std::string strSpawnArgs;
	for (const auto& arg : spawnArgs) {
		strSpawnArgs += arg + " ";
	}

	MS_lOGGERD("spawning worker process : {} {}", spawnBin, strSpawnArgs);

	char** args = new char*[spawnArgs.size()];
	for (int i = 0; i < spawnArgs.size(); i++) {
		args[i] = const_cast<char*>(spawnArgs[i].c_str());
	}
	args[spawnArgs.size()] = nullptr;
	
	uv_process_options_t options = {};
	options.file = spawnBin.c_str();
	options.args = args;

	// env
	char* env[2];
	env[0] = MEDIASOUP_VERSION;
	env[1] = nullptr;
	options.env = env;

	// ipc: whether this pipeline passes handles between different processes
	uv_pipe_init(Mediasoup::GetInstance().GetLoop(), &m_childStdOut, 0);
	uv_pipe_init(Mediasoup::GetInstance().GetLoop(), &m_childStdErr, 0);

	uv_pipe_init(Mediasoup::GetInstance().GetLoop(), &m_producer, 0);
	uv_pipe_init(Mediasoup::GetInstance().GetLoop(), &m_consumer, 0);	

	// options
	options.flags = UV_PROCESS_DETACHED;
	options.exit_cb = nullptr;

	// fd 0 (stdin)   : Just ignore it.
	// fd 1 (stdout)  : Pipe it for 3rd libraries that log their own stuff.
	// fd 2 (stderr)  : Same as stdout.
	// fd 3 (channel) : Producer Channel fd.
	// fd 4 (channel) : Consumer Channel fd.
	uv_stdio_container_t childStdio[5];
	childStdio[0].flags = UV_IGNORE;
	// UV_WRITABLE_PIPE/UV_READABLE_PIPE is relative to the child process
	childStdio[1].flags = (uv_stdio_flags)(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
	childStdio[1].data.stream = (uv_stream_t*)&m_childStdOut;
	childStdio[2].flags = (uv_stdio_flags)(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
	childStdio[2].data.stream = (uv_stream_t*)&m_childStdErr;
	childStdio[3].flags = (uv_stdio_flags)(UV_CREATE_PIPE | UV_READABLE_PIPE);
	childStdio[3].data.stream = (uv_stream_t*)&m_producer;
	childStdio[4].flags = (uv_stdio_flags)(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
	childStdio[4].data.stream = (uv_stream_t*)&m_consumer;
	options.stdio = childStdio;
	options.stdio_count = 5;
	
	int ret = uv_spawn(Mediasoup::GetInstance().GetLoop(), &m_child, &options);
	MS_ASSERTLOGE_R(0 == ret, false, "uv_spawn failed:{}", uv_strerror(ret));

	m_childStdOut.data = this;
	ret = uv_read_start((uv_stream_t*)&m_childStdOut, StaticOnAllocCB, StaticOnReadCB);
	MS_ASSERTLOGE_R(0 == ret, false, "uv_read_start failed:{}", uv_strerror(ret));

	m_childStdErr.data = this;
	ret = uv_read_start((uv_stream_t*)&m_childStdErr, StaticOnAllocCB, StaticOnReadCB);
	MS_ASSERTLOGE_R(0 == ret, false, "uv_read_start failed:{}", uv_strerror(ret));

	m_consumer.data = this;
	ret = uv_read_start((uv_stream_t*)&m_consumer, StaticOnAllocCB, StaticOnReadCB);
	MS_ASSERTLOGE_R(0 == ret, false, "uv_read_start failed:{}", uv_strerror(ret));

	m_pid = m_child.pid;

	return true;
}

void Worker::OnAllocCB(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf)
{
	if (handle == (uv_handle_t*)&m_childStdOut) {
		buf->base = m_stdOutBuf;
		buf->len = suggestedSize;
		m_stdOutReadLen = suggestedSize;
	}

	if (handle == (uv_handle_t*)&m_childStdErr) {
		buf->base = m_stdErrBuf;
		buf->len = suggestedSize;
		m_stdErrReadLen = suggestedSize;
	}

	if (handle == (uv_handle_t*)&m_consumer) {
		buf->base = m_consumerBuf;
		buf->len = suggestedSize;
		m_consumerReadLen = suggestedSize;
	}	
}

void Worker::OnReadCB(uv_stream_t* stream, ssize_t nRead, const uv_buf_t* buf)
{
	if (stream == (uv_stream_t*)&m_childStdOut) {
		m_stdOutBuf[nRead] = 0;
		MS_lOGGERD("OnReadCB:stdout:{}", m_stdOutBuf);
	}

	if (stream == (uv_stream_t*)&m_childStdErr) {
		m_stdErrBuf[nRead] = 0;
		MS_lOGGERD("OnReadCB:stderr:{}", m_stdErrBuf);
	}

	if (stream == (uv_stream_t*)&m_consumer) {
		m_consumerBuf[nRead] = 0;
		MS_lOGGERD("OnReadCB:consumer:{}", m_consumerBuf);
	}	
}

void Worker::getWorkBinPath() {
	MS_lOGGERF();

	if (!s_workerBin.empty()) {
		return;
	}

	std::string workerBinPath = Process::GetInstance().Environ("MEDIASOUP_WORKER_BIN");
	if (!workerBinPath.empty()) {
		s_workerBin = workerBinPath;
		return;
	}

	std::string strexePath = Process::GetInstance().ExePath();

	// replace all '\' with '/'
	while (true) {
		std::string::size_type pos(0);
		if ((pos = strexePath.find("\\")) != std::string::npos) {
			strexePath.replace(pos, 1, "/");
			continue;
		}
		break;
	}
	strexePath = strexePath.substr(0, strexePath.rfind('/') + 1);

	s_workerBin += strexePath + "mediasoup-worker";
}

}