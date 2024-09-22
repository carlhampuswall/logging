#pragma once
#include <cstring>
#include <stdarg.h>
#include <stdio.h>
#include <vector>

#include "logging/adapters/arduinoserial/arduino_serial_adapter.h"
#include "logging/adapters/log_adapter.h"

#include "structs.h"

#define LOG(log_level, verbose, ...)                                                                                   \
    Logging::getInstance()->log(log_level, verbose, millis(), __FILE__, __func__, __LINE__, __VA_ARGS__);

#define LOGI(...) LOG(LOG_LEVEL_INFO, false, __VA_ARGS__);
#define LOGW(...) LOG(LOG_LEVEL_WARNING, false, __VA_ARGS__);
#define LOGE(...) LOG(LOG_LEVEL_ERROR, false, __VA_ARGS__);
#define LOGD(...) LOG(LOG_LEVEL_DEBUG, false, __VA_ARGS__);

#define LOGV(log_level, ...) LOG(log_level, true, __VA_ARGS__);

// bool operator==(const LOG_MESSAGE &lhs, const LOG_MESSAGE &rhs);

class Logging {
  public:
    static void setAdapter(LogAdapter *adapter);
    static void setVerbose(bool verbose);
    static Logging *getInstance();
    void log(LOG_LEVEL level, bool verbose, unsigned long timestamp, char const *file, char const *func, int line,
             char const *fmt, ...);

  private:
    Logging() {};
    ~Logging() {};

    LogAdapter *adapter_ = new ArduinoSerialAdapter();

    Logging(const Logging &) = delete;
    Logging &operator=(const Logging &) = delete;
};