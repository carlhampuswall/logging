#pragma once
#include "stdio.h"

enum LOG_LEVEL { LOG_LEVEL_INFO = 1, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_DEBUG, LOG_LEVEL_COUNT };

#define LOG(log_level, verbose, ...)                                                                                   \
    do {                                                                                                               \
        Logging::getInstance()->log(log_level, verbose, __FILE__, __func__, __LINE__, __VA_ARGS__);                    \
    } while (0)

#define LOGI(...) LOG(LOG_LEVEL_INFO, false, __VA_ARGS__);
#define LOGW(...) LOG(LOG_LEVEL_WARNING, false, __VA_ARGS__);
#define LOGE(...) LOG(LOG_LEVEL_ERROR, false, __VA_ARGS__);
#define LOGD(...) LOG(LOG_LEVEL_DEBUG, false, __VA_ARGS__);

#define LOGV(log_level, ...) LOG(log_level, true, __VA_ARGS__);

struct LOG_MESSAGE {
    LOG_LEVEL level;
    bool verbose;
    const char *origin;
    const char *message;
};

class Logging {
  public:
    static Logging *getInstance();
    LOG_MESSAGE log(LOG_LEVEL level, bool verbose, const char *file, const char *func, int line, const char *fmt, ...);

  private:
    Logging() {};
    ~Logging() {};

    Logging(const Logging &) = delete;
    Logging &operator=(const Logging &) = delete;
};