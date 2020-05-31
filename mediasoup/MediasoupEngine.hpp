#pragma once
#include "IMediasoupEngine.hpp"
#include "common/Singleton.hpp"

namespace mediasoup
{

class MediasoupEngine : public IMediasoupEngine, public mediasoup::Singleton<MediasoupEngine> {
public:
    MediasoupEngine();
    virtual ~MediasoupEngine();

    virtual void Test() override;
	virtual bool Init() override;
    virtual void Destroy() override;
};

}