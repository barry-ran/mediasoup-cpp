#pragma once
#include "IWorker.hpp"
#include "CommonObserver.hpp"
#include "uv.h"
#include "UVPipeWrapper.hpp"

namespace mediasoup
{
class Worker : public IWorker,
	public CommonObserver<IWorker::Observer>,
	public UVPipeObserver {
public:
    Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings);
    virtual ~Worker();

	bool Init();

	// UVPipeObserver
	void OnRead(uint8_t* data, size_t len) override;
	void OnClose() override;

protected:
	void getWorkBinPath();
	
private:
	static std::string s_workerBin;
	WorkerSettings m_settings;
	uv_process_t m_child;

	std::unique_ptr<UVPipeWrapper> m_childStdOut = nullptr;
	std::unique_ptr<UVPipeWrapper> m_childStdErr = nullptr;

	int m_pid = -1;
};

}