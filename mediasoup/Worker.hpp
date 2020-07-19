#pragma once
#include "IWorker.hpp"
#include "CommonObserver.hpp"

namespace mediasoup
{
class Worker : public IWorker, public CommonObserver<IWorker::Observer> {
public:
    Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings);
    virtual ~Worker();

	bool Init();

private:
	void getWorkBinPath();

private:
	static std::string s_workerBin;
};

}