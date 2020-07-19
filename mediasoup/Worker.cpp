
#include "Worker.hpp"
#include "uv.h"
#include "Logger.hpp"

namespace mediasoup
{

// If env MEDIASOUP_WORKER_BIN is given, use it as worker binary.
// Otherwise if env MEDIASOUP_BUILDTYPE is 'Debug' use the Debug binary.
// Otherwise use the Release binary.
std::string Worker::s_workerBin = "";

Worker::Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings) {
	MS_lOGGERF();
	getWorkBinPath();
	MS_lOGGERI("mediasoup-worker path:{}", s_workerBin);

	RegisterObserver(obs);
	
	NotifyObserver(std::bind(&IWorker::Observer::OnSuccess, std::placeholders::_1));
}

Worker::~Worker() {
	MS_lOGGERF();
	UnregisterObserver();
}

void Worker::getWorkBinPath() {
	MS_lOGGERF();

	if (!s_workerBin.empty()) {
		return;
	}

	char workerBinPath[MAX_PATH] = { 0 };
	size_t workerBinPathLen = MAX_PATH;
	uv_os_getenv("MEDIASOUP_WORKER_BIN", workerBinPath, &workerBinPathLen);

	if (0 != strlen(workerBinPath)) {
		s_workerBin = workerBinPath;
		return;
	}

	char exePath[MAX_PATH] = { 0 };
	size_t exePathLen = MAX_PATH;
	uv_exepath(exePath, &exePathLen);

	std::string strexePath = exePath;

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