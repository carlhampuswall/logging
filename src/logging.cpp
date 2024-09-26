#include "logging.h"

void Logging::setAdapter(LogAdapter *adapter) { getInstance()->adapter_ = adapter; };

void Logging::setVerbose(bool verbose) { getInstance()->adapter_->setVerbose(verbose); }
void Logging::setOrigin(bool origin) { getInstance()->adapter_->setOrigin(origin); }

Logging *Logging::getInstance() {
    static Logging instance_;
    return &instance_;
}

void Logging::log(LOG_LEVEL level, bool verbose, unsigned long timesstamp, char const *file, char const *func, int line,
                  char const *fmt, ...) {
    LogMessage log_msg;
    log_msg.level = level;
    log_msg.verbose = verbose;
    log_msg.timestamp = timesstamp;

    if (level < 1 || level >= LOG_LEVEL_COUNT) {
        getInstance()->adapter_->handleLog(
            LogMessage{LOG_LEVEL_ERROR, false, timesstamp, "logging.cpp", "Invalid log level"});
    }

    snprintf(log_msg.origin, ORIGIN_MAX_SIZE, "%s:%d - %s", strrchr(file, '/') ? strrchr(file, '/') + 1 : file, line,
             func);
    log_msg.origin[ORIGIN_MAX_SIZE - 1] = '\0'; // Ensure null-termination

    va_list args;
    va_start(args, fmt);
    vsnprintf(log_msg.msg, MSG_MAX_SIZE, fmt, args);
    va_end(args);

    getInstance()->adapter_->handleLog(log_msg);
}