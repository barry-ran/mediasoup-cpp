#pragma once
#include "IWorker.hpp"
namespace mediasoup
{

class IMediasoupEngine {
  public:
    virtual ~IMediasoupEngine() {}

    virtual void Test() = 0;
	  virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual IWorker* CreateWorker() = 0;
};

mediasoup::IMediasoupEngine* CreateMediasoupEngine();

void DestroyMediasoupEngine(mediasoup::IMediasoupEngine *engine);

}