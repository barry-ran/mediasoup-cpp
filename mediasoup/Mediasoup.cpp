#include "Mediasoup.hpp"
#include "Logger.hpp"
#include "EventEmitter.hpp"
#include "Promise.hpp"
#include "Worker.hpp"
#include "SupportedRtpCapabilities.hpp"

using namespace promise;

namespace mediasoup
{

mediasoup::IMediasoup* CreateMediasoup() {
    MS_lOGGERF();
    return &mediasoup::Mediasoup::GetInstance();
}

void DestroyMediasoup(mediasoup::IMediasoup* mediasoup) {
    MS_lOGGERF();
    if (!mediasoup) {
        return;
    }
}

Mediasoup::Mediasoup() {
    MS_lOGGERF();
}

Mediasoup::~Mediasoup() {
    MS_lOGGERF();
}

void Mediasoup::Test() {
    return;
    MS_lOGGERD("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    MS_lOGGERW("Some error message with arg: {}", 1);
    MS_lOGGERE("Easy padding in numbers like {:08d}", 12);

    // test EventEmitter
    EventEmitter emitter;
    emitter.on(0, [](const char *text, int n) { 
        MS_lOGGERD("recv emitter evnet 0:{} {}", text, n);
    });

    MS_lOGGERD("emit evnet 0");
    emitter.emit(0, "from emit", 777);

    // test Promise
    newPromise([](Defer d){
        MS_lOGGERD("promise http request 1");
        int ret = 0;
        //ret = 1;
        if (0 == ret) {
            d.resolve();
        } else {
            d.reject(ret);
        }
    }).then([](){ // on resolve
        MS_lOGGERD("http request 1 success");
        Defer next = newPromise([](Defer d) {
            MS_lOGGERD("promise http request 2");
            int ret = 0;
            ret = 1;
            if (0 == ret) {
                d.resolve();
            } else {
                d.reject(ret);
            }
        });
        //Will call next.resole() or next.reject() later
        return next;
    }, [](int ret){ // on reject
        MS_lOGGERE("http request 1 failed:{}", ret);
    }).then([](){ // on resolve
        MS_lOGGERE("http request 2 success");
    }, [](int ret){ // on reject
        MS_lOGGERE("http request 2 failed:{}", ret);
    }).always([](){
        MS_lOGGERD("http request finish");
    });
}

void StaticWorkerFun(void* arg) {
    MS_lOGGERF();
    static_cast<Mediasoup*>(arg)->WorkerFun();
}

void StaticAsync(uv_async_t* handle) {
    MS_lOGGERF();
    static_cast<Mediasoup*>(handle->loop->data)->Async(handle);
}

bool Mediasoup::Init() {
    MS_lOGGERF();
    if (m_workThreadCreated) {
        MS_lOGGERI("already Init");
        return true;
    }

    int ret = uv_thread_create(&m_workThread, StaticWorkerFun, this);
    if (0 != ret) {
        MS_lOGGERE("uv_thread_create failed {}", ret);
        return false;
    }

    m_workThreadCreated = true;
    
	return true;
}

void Mediasoup::Destroy()  {
    MS_lOGGERF();
    if (!m_workThreadCreated) {
        MS_lOGGERI("need Init first");
        return;
    }

    // notify quit
    uv_async_send(&m_async);

    MS_lOGGERI("wait work thread quit");
    uv_thread_join(&m_workThread);
    m_workThreadCreated = false;

	// clear works
	for (const auto& it : m_works) {
		if (!it) {
			continue;
		}
		delete it;
	}

    MS_lOGGERI("work thread quit");
}

IWorker* Mediasoup::CreateWorker(IWorker::Observer* workerObserver, const WorkerSettings& workerSettings)  {
    MS_lOGGERF();
	Worker* work = new Worker(workerObserver, workerSettings);
	if (!work->Init()) {
		MS_lOGGERE("worker init failed");
		delete work;
		return nullptr;
	}

	m_works.push_back(work);
    return work;
}


RtpCapabilities Mediasoup::GetSupportedRtpCapabilities() {
	return supportedRtpCapabilities;
}

uv_loop_t* Mediasoup::GetLoop()
{
	return m_loop;
}

void Mediasoup::WorkerFun() {
    MS_lOGGERF();
    MS_lOGGERI("WorkerFun begine");

	m_loop = new uv_loop_t;
    if (nullptr == m_loop) {
        MS_lOGGERE("create loop failed");
        return;
    }

    uv_loop_init(m_loop);
    
    // save this
	m_loop->data = static_cast<void*>(this);

    uv_async_init(m_loop, &m_async, StaticAsync);

    MS_lOGGERI("uv_run");
    uv_run(m_loop, UV_RUN_DEFAULT);

    uv_loop_close(m_loop);
    delete m_loop;
	m_loop = nullptr;

    MS_lOGGERI("WorkerFun end");
}

void Mediasoup::Async(uv_async_t* handle) {
    MS_lOGGERF();
    uv_stop(handle->loop);
}

}