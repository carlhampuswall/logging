#include "logging.h"

Logging *Logging::getInstance() {
    static Logging instance_;
    return &instance_;
}

LOG_MESSAGE Logging::log(LOG_LEVEL level, bool verbose, const char *file, const char *func, int line, const char *fmt,
                         ...) {
    if (level < 1 || level >= LOG_LEVEL_COUNT) {
        return;
    }

    char origin_[128];
    snprintf(origin_, sizeof(origin_), "%s:%s:%d", file, func, line);

    va_list args;
    va_start(args, fmt);
    int required_size = vsnprintf(nullptr, 0, fmt, args) + 1;
    va_end(args);

    char *msg_ = new char[required_size];
    va_start(args, fmt);
    vsnprintf(msg_, required_size, fmt, args);
    va_end(args);

    return LOG_MESSAGE{level, verbose, origin_, msg_};
    // queueLogMsg(log_message);
}