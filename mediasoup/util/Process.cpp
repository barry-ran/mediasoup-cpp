
#include "Process.hpp"
#include "Logger.hpp"

namespace mediasoup
{

Process::Process() {
	MS_lOGGERF();
	int ret = uv_os_environ(&m_envItem, &m_envCount);
	MS_ASSERTLOGE_RV(0 == ret, "uv_os_environ failed:{}", ret);

	for (int i = 0; i < m_envCount; i++) {
		m_mapEnvs[m_envItem[i].name] = m_envItem[i].value;
	}
}

Process::~Process() {
	MS_lOGGERF();
	if (m_envItem) {
		uv_os_free_environ(m_envItem, m_envCount);
		m_envItem = nullptr;
		m_envCount = 0;
	}
	m_mapEnvs.clear();
}

const std::string& Process::Environ(const std::string& name) {
	MS_lOGGERF();
	if (m_mapEnvs.find(name) == m_mapEnvs.end()) {
		return m_emptyEnv;
	}
    
	return m_mapEnvs[name];
}

const std::string& Process::ExePath() {
	MS_lOGGERF();
	if (m_exePath.empty()) {
		char exePath[MAX_PATH] = { 0 };
		size_t exePathLen = MAX_PATH;
		uv_exepath(exePath, &exePathLen);
		m_exePath = exePath;
	}

	return m_exePath;
}

}