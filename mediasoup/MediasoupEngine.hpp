#pragma once
#include "IMediasoupEngine.hpp"
#include "common/Singleton.hpp"
#include "uv.h"

namespace mediasoup
{

class MediasoupEngine : public IMediasoupEngine, public mediasoup::Singleton<MediasoupEngine> {
public:
    MediasoupEngine();
    virtual ~MediasoupEngine();

    virtual void Test() override;
	virtual bool Init() override;
    virtual void Destroy() override;
    virtual IWorker* CreateWorker() override;

public:
    void WorkerFun();
    void Async(uv_async_t *handle);

private:
    uv_thread_t m_workThread;
    bool m_workThreadCreated = false;
    uv_async_t m_async = {};
    
};

}