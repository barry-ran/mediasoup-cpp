#pragma once
#include <vector>

#include "IMediasoup.hpp"
#include "common/Singleton.hpp"
#include "uv.h"

namespace mediasoup
{

class Mediasoup : public IMediasoup, public mediasoup::Singleton<Mediasoup> {
public:
    Mediasoup();
    virtual ~Mediasoup();

    virtual void Test() override;
	virtual bool Init() override;
    virtual void Destroy() override;
    virtual IWorker* CreateWorker(IWorker::Observer* workerObserver, const WorkerSettings& workerSettings) override;
	virtual RtpCapabilities GetSupportedRtpCapabilities() override;

public:
    void WorkerFun();
    void Async(uv_async_t* handle);

private:
    uv_thread_t m_workThread;
    bool m_workThreadCreated = false;
    uv_async_t m_async = {};

	std::vector<IWorker*> m_works;
    
};

}