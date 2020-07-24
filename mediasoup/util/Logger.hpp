#pragma once

#include "Singleton.hpp"
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

#define MS_lOGD(...) mediasoup::Logger::GetInstance().debug(__VA_ARGS__)
#define MS_lOGI(...) mediasoup::Logger::GetInstance().info(__VA_ARGS__)
#define MS_lOGW(...) mediasoup::Logger::GetInstance().warn(__VA_ARGS__)
#define MS_lOGE(...) mediasoup::Logger::GetInstance().error(__VA_ARGS__)
#define MS_lOGF() mediasoup::Logger::GetInstance().debug(SPDLOG_FUNCTION)

#define MS_ASSERT_RV_LOGI(x, r, ...) \
  do {                           \
    if (!(x)) {                  \
      MS_lOGI(__VA_ARGS__);   \
      return (r);                \
    }                            \
  } while (0);

#define MS_ASSERT_RV_LOGW(x, r, ...) \
  do {                           \
    if (!(x)) {                  \
      MS_lOGW(__VA_ARGS__);   \
      return (r);                \
    }                            \
  } while (0);

#define MS_ASSERT_RV_LOGE(x, r, ...) \
  do {                           \
    if (!(x)) {                  \
      MS_lOGE(__VA_ARGS__);   \
      return (r);                \
    }                            \
  } while (0);

#define MS_ASSERT_R_LOGI(x, ...) \
  do {                           \
    if (!(x)) {                  \
      MS_lOGI(__VA_ARGS__);   \
      return;                    \
    }                            \
  } while (0);

#define MS_ASSERT_R_LOGW(x, ...) \
  do {                           \
    if (!(x)) {                  \
      MS_lOGW(__VA_ARGS__);   \
      return;                    \
    }                            \
  } while (0);

#define MS_ASSERT_R_LOGE(x, ...) \
  do {                           \
    if (!(x)) {                  \
      MS_lOGE(__VA_ARGS__);   \
      return;                    \
    }                            \
  } while (0);

}