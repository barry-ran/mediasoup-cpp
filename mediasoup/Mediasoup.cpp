#include "Mediasoup.hpp"
#include "Logger.hpp"
#include "Promise.hpp"
#include "Worker.hpp"
#include "SupportedRtpCapabilities.hpp"

using namespace promise;

namespace mediasoup
{

mediasoup::IMediasoup* CreateMediasoup() {
    MS_lOGF();
    return &mediasoup::Mediasoup::GetInstance();
}

void DestroyMediasoup(mediasoup::IMediasoup* mediasoup) {
    MS_lOGF();
    if (!mediasoup) {
        return;
    }
}

Mediasoup::Mediasoup() {
    MS_lOGF();
}

Mediasoup::~Mediasoup() {
    MS_lOGF();
}

void Mediasoup::Test() {
    return;
    MS_lOGD("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    MS_lOGW("Some error message with arg: {}", 1);
    MS_lOGE("Easy padding in numbers like {:08d}", 12);    

    // test Promise
    newPromise([](Defer d){
        MS_lOGD("promise http request 1");
        int ret = 0;
        //ret = 1;
        if (0 == ret) {
            d.resolve();
        } else {
            d.reject(ret);
        }
    }).then([](){ // on resolve
        MS_lOGD("http request 1 success");
        Defer next = newPromise([](Defer d) {
            MS_lOGD("promise http request 2");
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
        MS_lOGE("http request 1 failed:{}", ret);
    }).then([](){ // on resolve
        MS_lOGE("http request 2 success");
    }, [](int ret){ // on reject
        MS_lOGE("http request 2 failed:{}", ret);
    }).always([](){
        MS_lOGD("http request finish");
    });
}

void StaticWorkerFun(void* arg) {
    MS_lOGF();
    static_cast<Mediasoup*>(arg)->WorkerFun();
}

void StaticAsync(uv_async_t* handle) {
    MS_lOGF();
    static_cast<Mediasoup*>(handle->loop->data)->Async(handle);
}

bool Mediasoup::Init() {
    MS_lOGF();
	MS_ASSERT_RV_LOGI(!m_loop, true, "already Init");

	m_loop = new uv_loop_t;
	MS_ASSERT_RV_LOGE(m_loop, false, "create loop failed");

	uv_loop_init(m_loop);

	// save this
	m_loop->data = static_cast<void*>(this);

	uv_async_init(m_loop, &m_async, StaticAsync);

	// run loop
    int ret = uv_thread_create(&m_workThread, StaticWorkerFun, this);
    if (0 != ret) {
        MS_lOGE("uv_thread_create failed:{}", uv_strerror(ret));
		Destroy();
        return false;
    }    
    
	return true;
}

void Mediasoup::Destroy()  {
    MS_lOGF();
	MS_ASSERT_R_LOGI(m_loop, "need Init first");

    // notify quit
    uv_async_send(&m_async);

    MS_lOGI("wait work thread quit");
    uv_thread_join(&m_workThread); 

	// clear works
	for (const auto& it : m_works) {
		if (!it) {
			continue;
		}
		delete it;
	}

	uv_loop_close(m_loop);
	delete m_loop;
	m_loop = nullptr;

    MS_lOGI("work thread quit");
}

IWorker* Mediasoup::CreateWorker(IWorker::Observer* workerObserver, const WorkerSettings& workerSettings)  {
    MS_lOGF();
	Worker* work = new Worker(workerObserver, workerSettings);
	if (!work->Init()) {
		MS_lOGE("worker init failed");
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
    MS_lOGI("WorkerFun begine");
	
    MS_lOGI("uv_run");
    uv_run(m_loop, UV_RUN_DEFAULT);    

    MS_lOGI("WorkerFun end");
}

void Mediasoup::Async(uv_async_t* handle) {
    MS_lOGF();
    uv_stop(handle->loop);
}

}