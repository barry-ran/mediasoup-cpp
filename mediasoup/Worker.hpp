#pragma once
#include "IWorker.hpp"
#include "common/CommonObserver.hpp"

namespace mediasoup
{
class Worker : public IWorker, public CommonObserver<IWorker::Observer> {
public:
    Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings);
    virtual ~Worker();

private:
	void getWorkBinPath();

private:
	static std::string s_workerBin;
};

}