#include "Channel.hpp"
#include "Logger.hpp"
extern "C"
{
#include "netstring.h"
}

namespace mediasoup
{

// netstring length for a 4194304 bytes payload.
const static size_t NS_MESSAGE_MAX_LEN = 4194313;
const static size_t NS_PAYLOAD_MAX_LEN = 4194304;

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
    m_readBuf.append((char*)data, len);
	MS_lOGD("Channel::OnRead:{}", m_readBuf);

	// once
	if (!m_once && m_readBuf.find("running") != std::string::npos) {
		m_once = true;
		NotifyObserver(std::bind(&ChannelObserver::OnRunning, std::placeholders::_1));
		m_readBuf.clear();
		return;
	}
	
	if (m_readBuf.length() > NS_PAYLOAD_MAX_LEN) {
		MS_lOGE("receiving buffer is full, discarding all data into it");
		// Reset the buffer and exit.
		m_readBuf.clear();
		return;
	}

	// Be ready to parse more than a single message in a single TCP chunk.
	while (true) {
		size_t readLen = m_readBuf.length();
		char* jsonStart = nullptr;
		size_t jsonLen;
		int nsRet = netstring_read(const_cast<char*>(m_readBuf.c_str()), readLen, &jsonStart, &jsonLen);

		if (nsRet != 0) {
			switch (nsRet) {
			case NETSTRING_ERROR_TOO_SHORT:
			{
				// message incomplete is not full, just wait.
				MS_lOGI("NETSTRING_ERROR_TOO_SHORT");
				return;
			}
			case NETSTRING_ERROR_TOO_LONG:
			{
				MS_lOGE("NETSTRING_ERROR_TOO_LONG");
				break;
			}
			case NETSTRING_ERROR_NO_COLON:
			{
				MS_lOGE("NETSTRING_ERROR_NO_COLON");
				break;
			}
			case NETSTRING_ERROR_NO_COMMA:
			{
				MS_lOGE("NETSTRING_ERROR_NO_COMMA");
				break;
			}
			case NETSTRING_ERROR_LEADING_ZERO:
			{
				MS_lOGE("NETSTRING_ERROR_LEADING_ZERO");
				break;
			}
			case NETSTRING_ERROR_NO_LENGTH:
			{
				MS_lOGE("NETSTRING_ERROR_NO_LENGTH");
				break;
			}

			}

			// Error, so reset and exit the parsing loop.
			m_readBuf.clear();
			return;
		}

		try {
			nlohmann::json jsonMessage = nlohmann::json::parse(jsonStart, jsonStart + jsonLen);
			processMessage(jsonMessage);

			if (jsonLen == m_readBuf.length()) {
				m_readBuf.clear();
				return;
			} else {
				m_readBuf = m_readBuf.substr(jsonLen, m_readBuf.length() - jsonLen);
				continue;
			}
			
		} catch (const nlohmann::json::parse_error& error) {
			MS_lOGE("JSON parsing error: %s", error.what());
			m_readBuf.clear();
			return;
		}

		return;
	}
}

void Channel::processMessage(const nlohmann::json& jsonMessage) {

}

void Channel::OnClose(UVPipeWrapper * pipe) {
}

}