#pragma once
#include "IWorker.hpp"
#include "CommonObserver.hpp"
#include "uv.h"

namespace mediasoup
{
class Worker : public IWorker, public CommonObserver<IWorker::Observer> {
public:
    Worker(IWorker::Observer* obs, const WorkerSettings& workerSettings);
    virtual ~Worker();

	bool Init();

	void OnAllocCB(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf);
	void OnReadCB(uv_stream_t* stream, ssize_t nRead, const uv_buf_t* buf);

protected:
	void getWorkBinPath();
	
private:
	static std::string s_workerBin;
	WorkerSettings m_settings;
	uv_process_t m_child;

	uv_pipe_t m_childStdOut;
	char m_stdOutBuf[1024] = { 0 };
	int m_stdOutReadLen = 0;
	uv_pipe_t m_childStdErr;
	char m_stdErrBuf[1024] = { 0 };
	int m_stdErrReadLen = 0;

	uv_pipe_t m_producer;
	uv_pipe_t m_consumer;
	char m_consumerBuf[1024] = { 0 };
	int m_consumerReadLen = 0;


	int m_pid = -1;
};

}