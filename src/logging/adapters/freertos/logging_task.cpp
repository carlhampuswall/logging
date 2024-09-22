#include "logging_task.h"

void LoggingTask::run() {
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