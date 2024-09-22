#include "logging_task.h"

void LoggingTask::run() {

    // SerialProtocolPlaintext serial_protocol_(stream_);
    // protocol_ = &serial_protocol_;

    // Delay to allow stream to initialize
    delay(1000);

    while (1) {
        if (xQueueReceive(log_queue, &log_msg, portMAX_DELAY)) {
            protocol_->log(log_msg);
        }
        delay(1);
    }
}

void LoggingTask::enqueue_log(LogMessage log_message) { xQueueSend(log_queue, &log_message, 0); }