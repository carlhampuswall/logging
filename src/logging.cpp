#include "logging.h"

void Logging::setAdapter(LogAdapter *adapter) {
    if (adapter == nullptr) {
        return;
    }

    uint32_t startTime = millis(); // Track when the attempt started

    // Retry mechanism: check if the adapter is available, retry up to 5 seconds
    while (!adapter->available()) {
        vTaskDelay(pdMS_TO_TICKS(100)); // Use FreeRTOS-friendly delay (100 ms intervals)

        // Timeout after 5 seconds (5000 ms)
        if (millis() - startTime > 5000) {
            Serial.println("Error: Adapter not available after 5 seconds.");
            return; // Exit the function if the adapter doesn't become available
        }
    }

    // If the adapter is available, set it
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

    LogMessage log_msg;
    log_msg.level = level;
    log_msg.verbose = verbose;
    log_msg.timestamp = timesstamp;

    if (level < 1 || level >= LOG_LEVEL_COUNT) {
        log->adapter_->handleLog(LogMessage{LOG_LEVEL_ERROR, false, timesstamp, "logging.cpp", "Invalid log level"});
    }

    snprintf(log_msg.origin, ORIGIN_MAX_SIZE, "%s:%d - %s", strrchr(file, '/') ? strrchr(file, '/') + 1 : file, line,
             func);
    log_msg.origin[ORIGIN_MAX_SIZE - 1] = '\0'; // Ensure null-termination

    va_list args;
    va_start(args, fmt);
    vsnprintf(log_msg.msg, MSG_MAX_SIZE, fmt, args);
    va_end(args);

    log->adapter_->handleLog(log_msg);
}