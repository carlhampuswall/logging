#include "serial_protocol_plaintext.h"

void SerialProtocolPlaintext::registerKeyHandler(char key, KeyCallback callback) { keyHandlers_[key] = callback; }

void SerialProtocolPlaintext::log(LogMessage *log_msg) {
    if (!verbose_ && log_msg->verbose) {
        return;
    }

    std::string msg_ = "";

    if (log_msg->verbose) {
        msg_ += "[V]";
    }

    switch (log_msg->level) {
    case LOG_LEVEL_INFO:
        msg_ += LOGGING_GRAY_TEXT "[INFO] " LOGGING_RESET_COLOR;
        break;
    case LOG_LEVEL_WARNING:
        msg_ += LOGGING_YELLOW_TEXT "[WARNING] " LOGGING_RESET_COLOR;
        break;
    case LOG_LEVEL_ERROR:
        msg_ += LOGGING_RED_TEXT "[ERROR] " LOGGING_RESET_COLOR;
        break;
    case LOG_LEVEL_DEBUG:
        msg_ += LOGGING_GREEN_TEXT "[DEBUG] " LOGGING_RESET_COLOR;
        break;
    default:
        msg_ += LOGGING_GRAY_TEXT "[INFO] " LOGGING_RESET_COLOR;
        break;
    }

    msg_ += log_msg->msg;

    if (origin_) {
        msg_ += LOGGING_CYAN_TEXT " [" + std::string(log_msg->origin) + "]" LOGGING_RESET_COLOR;
    }
    stream_.println(msg_.c_str());
    free(log_msg);
}

void SerialProtocolPlaintext::log_raw(const char *msg) { stream_.println(msg); }

void SerialProtocolPlaintext::readSerial() {
    while (stream_.available() > 0) {
        int b = stream_.read();

        if (b != 0 && stream_.available() + 1 > 1) {
            // TODO Fixes bug or something that seems to ouput something to serial at conn to
            // TODO it if connections inst established directly at boot!?
            return;
        }

        if (keyHandlers_.find(b) != keyHandlers_.end()) {
            auto handler = new std::function<void()>(keyHandlers_[b]);
            xTaskCreate(
                [](void *param) {
                    auto handler = reinterpret_cast<std::function<void()> *>(param);
                    (*handler)();
                    delete handler;
                    vTaskDelete(NULL);
                },
                "key_handler_task", 1024 * 12, handler, 5, NULL); // TODO stack size and priority?
        } else {                                                  //! Default key handlers can be overwritten
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