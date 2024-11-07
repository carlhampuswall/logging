#include "logging.h"

void Logging::setAdapter(LogAdapter *adapter) {
    if (adapter == nullptr) {
        return;
    }

    uint32_t startTime = millis();

    while (!adapter->available()) {
        delay(100);

        if (millis() - startTime > 5000) {
            Serial.println("Error: Adapter not available after 5 seconds.");
            return;
        }
    }

    getInstance()->adapter_ = adapter;
}

void Logging::setVerbose(bool verbose) {
    Logging *log = getInstance();
    assert(log->adapter_);
    log->adapter_->setVerbose(verbose);
}
void Logging::setOrigin(bool origin) {
    Logging *log = getInstance();
    assert(log->adapter_);
    log->adapter_->setOrigin(origin);
}

Logging *Logging::getInstance() {
    static Logging instance_;
    return &instance_;
}

void Logging::log(LOG_LEVEL level, bool verbose, unsigned long timesstamp, char const *file, char const *func, int line,
                  char const *fmt, ...) {
    Logging *log = getInstance();
    assert(log->adapter_);

    // // LogMessage log_msg;
    LogMessage *log_msg = (LogMessage *)malloc(sizeof(LogMessage));
    log_msg->level = level;
    log_msg->verbose = verbose;
    log_msg->timestamp = timesstamp;

    if (level < 1 || level >= LOG_LEVEL_COUNT) {
        // LogMessage *invalid_log_level = new LogMessage{LOG_LEVEL_ERROR, false, timesstamp, "logging.cpp", "Invalid
        // log level"};
        log_msg->level = LOG_LEVEL_ERROR;
        log_msg->verbose = false;
        log_msg->timestamp = timesstamp;
        snprintf(log_msg->origin, ORIGIN_MAX_SIZE, "logging.cpp");
        snprintf(log_msg->msg, MSG_MAX_SIZE, "Invalid log level");

        log->adapter_->handleLog(log_msg);
        return;
    }

    snprintf(log_msg->origin, ORIGIN_MAX_SIZE, "%s:%d - %s", strrchr(file, '/') ? strrchr(file, '/') + 1 : file, line,
             func);
    log_msg->origin[ORIGIN_MAX_SIZE - 1] = '\0'; // Ensure null-termination

    va_list args;
    va_start(args, fmt);
    vsnprintf(log_msg->msg, MSG_MAX_SIZE, fmt, args);
    va_end(args);

    log->adapter_->handleLog(log_msg);
}