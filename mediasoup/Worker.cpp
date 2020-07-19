
#include "Worker.hpp"
#include "uv.h"
#include "Logger.hpp"
#include "Process.hpp"

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

bool Worker::Init()
{
	MS_lOGGERF();
	return true;
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