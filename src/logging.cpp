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

void Logging::log(LOG_LEVEL level, bool verbose, unsigned long timesstamp, const char *file, const char *func, int line,
                  const char *fmt, ...) {
    if (adapter_ == nullptr) {
        setAdapter(new ArduinoSerialAdapter());
        // return; // TODO: throw exception? for now set default adapter
    }

    if (level < 1 || level >= LOG_LEVEL_COUNT) {
        // return LOG_MESSAGE{LOG_LEVEL_ERROR, false, "logging.cpp", "Invalid log level"};
        adapter_->handle_log(LogMessage{LOG_LEVEL_ERROR, false, timesstamp, "logging.cpp", "Invalid log level"});
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

    adapter_->handle_log(LogMessage{level, verbose, timesstamp, origin_, msg_});
}