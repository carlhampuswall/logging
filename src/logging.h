#pragma once
#include <cstring>
#include <stdarg.h>
#include <stdio.h>
#include <vector>

#include "logging/adapters/arduinoserial/arduino_serial_adapter.h"
#include "logging/adapters/log_adapter.h"

#include "structs.h"

#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL LOG_LEVEL_INFO
#endif

#ifndef ENABLE_VERBOSE_LOGGING
#define ENABLE_VERBOSE_LOGGING true
#endif

#ifndef VERBOSE_LOGGING_DEFAULT
#define VERBOSE_LOGGING_DEFAULT false
#endif

#define LOG(...) LOG_WITH_LEVEL(DEFAULT_LOG_LEVEL, false, __VA_ARGS__);

#define LOG_WITH_LEVEL(log_level, verbose, ...)                                                                        \
    Logging::getInstance()->log(log_level, verbose, millis(), __FILE__, __func__, __LINE__, __VA_ARGS__);

#define LOGI(...) LOG_WITH_LEVEL(LOG_LEVEL_INFO, false, __VA_ARGS__);
#define LOGW(...) LOG_WITH_LEVEL(LOG_LEVEL_WARNING, false, __VA_ARGS__);
#define LOGE(...) LOG_WITH_LEVEL(LOG_LEVEL_ERROR, false, __VA_ARGS__);
#define LOGD(...) LOG_WITH_LEVEL(LOG_LEVEL_DEBUG, false, __VA_ARGS__);

#if ENABLE_VERBOSE_LOGGING
#define LOGV(log_level, ...) LOG_WITH_LEVEL(log_level, true, __VA_ARGS__);
#else
#define LOGV(log_level, ...) ; // Do nothing
#endif

// bool operator==(const LOG_MESSAGE &lhs, const LOG_MESSAGE &rhs);

class Logging {

  public:
    static void setAdapter(LogAdapter *adapter);
    static void setVerbose(bool verbose);
    static Logging *getInstance();
    static void log(LOG_LEVEL level, bool verbose, unsigned long timestamp, char const *file, char const *func,
                    int line, char const *fmt, ...);

  private:
    Logging() {};
    ~Logging() {};

    LogAdapter *adapter_ = new ArduinoSerialAdapter();

    Logging(const Logging &) = delete;
    Logging &operator=(const Logging &) = delete;
};