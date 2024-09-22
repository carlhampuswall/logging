#include "serial_protocol_plaintext.h"

void SerialProtocolPlaintext::registerKeyHandler(char key, KeyCallback callback) { keyHandlers_[key] = callback; }

void SerialProtocolPlaintext::log(LogMessage log_msg) {
    if (!verbose_ && log_msg.verbose) {
        return;
    }

    if (origin_) {
        stream_.printf("[%s]", log_msg.origin);
    }

    switch (log_msg.level) {
    case LOG_LEVEL_INFO:
        stream_.print("\u001b[37m INFO:\033[0m ");
        break;
    case LOG_LEVEL_WARNING:
        stream_.print("\u001b[33m WARNING:\033[0m ");
        break;
    case LOG_LEVEL_ERROR:
        stream_.print("\u001b[31m ERROR:\033[0m ");
        break;
    case LOG_LEVEL_DEBUG:
        stream_.print("\u001b[32m DEBUG:\033[0m ");
        break;
    default:
        stream_.print("\u001b[37m  INFO:\033[0m ");
        break;
    }
    stream_.println(log_msg.msg);
    stream_.flush();
}

void SerialProtocolPlaintext::readSerial() {
    while (stream_.available() > 0) {
        int b = stream_.read();

        if (keyHandlers_.find(b) != keyHandlers_.end()) {
            keyHandlers_[b]();
        } else { // Default key handlers can be overwritten
            if (b == 'V' || b == 'v') {
                LOGI("Verbose logging toggle request received");
                setVerbose(!verbose_);
            } else if (b == 'O' || b == 'o') {
                LOGI("Origin logging toggle request received");
                setOrigin(!origin_);
            }
        }
    }
}