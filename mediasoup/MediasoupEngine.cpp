#include "MediasoupEngine.hpp"
#include "Logger.hpp"
#include "EventEmitter.hpp"
#include "Promise.hpp"

using namespace promise;

namespace mediasoup
{

mediasoup::IMediasoupEngine* CreateMediasoupEngine() {
    MS_lOGGERD("CreateMediasoupEngine");
    return &mediasoup::MediasoupEngine::GetInstance();
}

void DestroyMediasoupEngine(mediasoup::IMediasoupEngine *engine) {
    if (!engine) {
        return;
    }
    MS_lOGGERD("DestroyMediasoupEngine");
}

MediasoupEngine::MediasoupEngine() {

}

MediasoupEngine::~MediasoupEngine() {
   
}

void MediasoupEngine::Test() {
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

bool MediasoupEngine::Init() {
	return true;
}

void MediasoupEngine::Destroy()  {

}

}