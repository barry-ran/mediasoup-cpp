#pragma once
#include "IWorker.hpp"
namespace mediasoup
{

class IMediasoup {
  public:
    virtual ~IMediasoup() {}

    virtual void Test() = 0;
	  virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual IWorker* CreateWorker(const WorkerSettings& workerSettings) = 0;
};

mediasoup::IMediasoup* CreateMediasoup();

void DestroyMediasoup(mediasoup::IMediasoup* mediasoup);

}