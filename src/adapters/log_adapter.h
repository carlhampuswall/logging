#pragma once
#include "../structs.h"

class LogAdapter {
  public:
    LogAdapter() {};
    virtual ~LogAdapter() {};

    virtual void output(const char *message) = 0;
    virtual void handle_log(LogMessage log_message) = 0;

    virtual void setVerbose(bool verbose) { verbose_ = verbose; }
    virtual void setOrigin(bool origin) { origin_ = origin; }

  protected:
    std::string logLevelToString(LOG_LEVEL level) {
        switch (level) {
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARNING:
            return "WARNING";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        default:
            return "UNKNOWN";
        }
    }

    bool verbose_ = false;
    bool origin_ = false;
};