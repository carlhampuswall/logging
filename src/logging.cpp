#include "logging.h"

// bool operator==(const LOG_MESSAGE &lhs, const LOG_MESSAGE &rhs) {
//     return lhs.level == rhs.level && lhs.verbose == rhs.verbose && strcmp(lhs.origin, rhs.origin) == 0 &&
//            strcmp(lhs.message, rhs.message) == 0;
// }

void Logging::setAdapter(LogAdapter *adapter) { getInstance()->adapter_ = adapter; };

void Logging::setVerbose(bool verbose) { getInstance()->adapter_->setVerbose(verbose); }

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
        adapter_->handleLog(LogMessage{LOG_LEVEL_ERROR, false, timesstamp, "logging.cpp", "Invalid log level"});
    }

    snprintf(log_msg.origin, ORIGIN_MAX_SIZE, "%s : %s : %d", file, func, line);
    log_msg.origin[ORIGIN_MAX_SIZE - 1] = '\0'; // Ensure null-termination

    va_list args;
    va_start(args, fmt);
    vsnprintf(log_msg.msg, MSG_MAX_SIZE, fmt, args);
    va_end(args);

    adapter_->handleLog(log_msg);
}