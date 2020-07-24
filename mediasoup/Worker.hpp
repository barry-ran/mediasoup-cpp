#pragma once
#include "IWorker.hpp"
#include "CommonObserver.hpp"
#include "uv.h"
#include "UVPipeWrapper.hpp"
#include "Channel.hpp"

namespace mediasoup
{
class Worker : public IWorker
	, public CommonObserver<IWorker::Observer>
	, public UVPipeObserver 
	, public ChannelObserver {
public:
    Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings);
    virtual ~Worker();

	bool Init();
	void Close();

	void OnExit(int64_t exitStatus, int termSignal);
	// UVPipeObserver
	void OnRead(UVPipeWrapper* pipe, uint8_t* data, size_t len) override;
	void OnClose(UVPipeWrapper* pipe) override;

	// ChannelObserver
	void OnRunning();
	void OnMsg(std::string targetId, std::string event, std::string data) override;

protected:
	void getWorkBinPath();
	
private:
	static std::string s_workerBin;
	WorkerSettings m_settings;
	uv_process_t* m_child = nullptr;

	std::unique_ptr<UVPipeWrapper> m_childStdOut = nullptr;
	std::unique_ptr<UVPipeWrapper> m_childStdErr = nullptr;
	std::unique_ptr<Channel> m_channel = nullptr;

	int m_pid = -1;
	bool m_spawnDone = false;
};

}