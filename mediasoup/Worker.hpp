#pragma once
#include "IWorker.hpp"

namespace mediasoup
{
class Worker : public IWorker {
public:
    Worker(const WorkerSettings& workerSettings);
    virtual ~Worker();

private:
	void getWorkBinPath();

private:
	static std::string s_workerBin;
};

}