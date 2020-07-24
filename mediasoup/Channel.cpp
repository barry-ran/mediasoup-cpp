#include "Channel.hpp"
#include "Logger.hpp"

namespace mediasoup
{

Channel::Channel(ChannelObserver* obs) {
	MS_lOGF();
	RegisterObserver(obs);
	m_producerPipe.reset(new UVPipeWrapper(this, 1024, UVPipeWrapper::Role::PRODUCER));
	m_consumerPipe.reset(new UVPipeWrapper(this, 1024, UVPipeWrapper::Role::CONSUMER));
}

Channel::~Channel() {
	MS_lOGF();
}

bool Channel::Init() {
	MS_lOGF();
	MS_ASSERT_RV_LOGE(m_producerPipe->Init(), false, "m_producerPipe init failed");
	MS_ASSERT_RV_LOGE(m_consumerPipe->Init(), false, "m_consumerPipe init failed");
	
	return true;
}
bool Channel::Start() {
	MS_lOGF();
	MS_ASSERT_RV_LOGE(m_consumerPipe->Start(), false, "m_consumerPipe start failed");
	
	return true;
}

void Channel::SetPid(int pid) {
	m_pid = pid;
}

UVPipeWrapper * Channel::GetProducer() {
	return m_producerPipe.get();
}

UVPipeWrapper * Channel::GetConsumer() {
	return m_consumerPipe.get();
}

void Channel::OnRead(UVPipeWrapper * pipe, uint8_t * data, size_t len) {
	std::string s((char*)data, len);
	MS_lOGD("Channel::OnRead: {}", s);
}

void Channel::OnClose(UVPipeWrapper * pipe) {
}

}