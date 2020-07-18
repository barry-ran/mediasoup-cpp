#pragma once

#include "common/Singleton.hpp"
#include "spdlog/spdlog.h"

namespace mediasoup
{

class Logger : public mediasoup::Singleton<Logger> {
public:
    Logger();
    ~Logger();

    template <typename... Args>
    void trace(const Args&... args) {
	    spdlog::trace(args...);
    }

	template <typename... Args>
    void debug(const Args&... args) {
	    spdlog::debug(args...);
    }

    template <typename... Args>
    void info(const Args&... args) {
	    spdlog::info(args...);
    }

    template <typename... Args>
    void warn(const Args&... args) {
	    spdlog::warn(args...);
    }

    template <typename... Args>
    void error(const Args&... args) {
	    spdlog::error(args...);
    }
};

#define MS_lOGGERD(...) mediasoup::Logger::GetInstance().debug(__VA_ARGS__)
#define MS_lOGGERI(...) mediasoup::Logger::GetInstance().info(__VA_ARGS__)
#define MS_lOGGERW(...) mediasoup::Logger::GetInstance().warn(__VA_ARGS__)
#define MS_lOGGERE(...) mediasoup::Logger::GetInstance().error(__VA_ARGS__)
#define MS_lOGGERF() mediasoup::Logger::GetInstance().debug(SPDLOG_FUNCTION)

}