#pragma once
#include <memory>
#include <string>

#include "UVPipeWrapper.hpp"
#include "CommonObserver.hpp"

namespace mediasoup
{

class ChannelObserver {
public:
	virtual void OnMsg(std::string targetId, std::string event, std::string data) = 0;
};

class Channel : public CommonObserver<ChannelObserver>, public UVPipeObserver {
public:
    Channel(ChannelObserver* obs);
    virtual ~Channel();

	bool Init();
	bool Start();
	void SetPid(int pid);
	UVPipeWrapper* GetProducer();
	UVPipeWrapper* GetConsumer();

	// UVPipeObserver
	void OnRead(UVPipeWrapper* pipe, uint8_t* data, size_t len) override;
	void OnClose(UVPipeWrapper* pipe) override;

private:
	int m_pid = -1;
	std::unique_ptr<UVPipeWrapper> m_producerPipe = nullptr;
	std::unique_ptr<UVPipeWrapper> m_consumerPipe = nullptr;
};

}