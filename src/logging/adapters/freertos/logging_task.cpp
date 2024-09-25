#include "logging_task.h"

#ifndef ENABLE_LOGGING_INIT_MESSAGE
#define ENABLE_LOGGING_INIT_MESSAGE true
#endif

#ifndef LOGGING_CUSTOM_INIT_TITLE
#define LOGGING_CUSTOM_INIT_TITLE "LOGGING TASK INITIALIZED"
#endif

void LoggingTask::run() {
#if ENABLE_LOGGING_INIT_MESSAGE
    protocol_->log_raw(BOLD_TEXT BLUE_TEXT "*******************************************" RESET_COLOR);
    protocol_->log_raw(BOLD_TEXT BLUE_TEXT "*   " LOGGING_CUSTOM_INIT_TITLE RESET_COLOR);
    protocol_->log_raw(BOLD_TEXT BLUE_TEXT "*******************************************" RESET_COLOR);
    protocol_->log_raw("");
    protocol_->log_raw(GREEN_TEXT "Press 'V' to toggle verbose logging" RESET_COLOR);
    protocol_->log_raw(GREEN_TEXT "Press 'O' to toggle origin logging" RESET_COLOR);
    protocol_->log_raw(YELLOW_TEXT "[V] = Verbose logging indicator" RESET_COLOR);
    protocol_->log_raw("");
#if ENABLE_VERBOSE_LOGGING
#if VERBOSE_LOGGING_DEFAULT
    protocol_->log_raw(BOLD_TEXT "Verbose logging is currently: " GREEN_TEXT "ENABLED" RESET_COLOR);
#else
    protocol_->log_raw(BOLD_TEXT "Verbose logging is currently: " YELLOW_TEXT "DISABLED" RESET_COLOR);
#endif
#else
    protocol_->log_raw(BOLD_TEXT "Verbose logging is currently: " RED_TEXT "NOT SUPPORTED" RESET_COLOR);
#endif
    protocol_->log_raw("");
    protocol_->log_raw(BOLD_TEXT BLUE_TEXT "*******************************************" RESET_COLOR);
    protocol_->log_raw("");
#endif

    while (1) {
        if (xQueueReceive(log_queue, &log_msg, 0)) {
            protocol_->log(log_msg);
        }
        protocol_->read();
        delay(1);
    }
}

void LoggingTask::enqueue_log(const LogMessage &log_message) {
    if (xQueueSend(log_queue, &log_message, pdMS_TO_TICKS(10)) != pdTRUE) {
        throw std::runtime_error("Failed to send log message to queue");
    }
}