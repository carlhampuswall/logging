#include "logging_task.h"

#ifndef ENABLE_LOGGING_INIT_MESSAGE
#define ENABLE_LOGGING_INIT_MESSAGE true
#endif

#ifndef LOGGING_CUSTOM_INIT_TITLE
#define LOGGING_CUSTOM_INIT_TITLE "LOGGING TASK INITIALIZED"
#endif

#ifndef LOGGING_DELAY
#define LOGGING_DELAY 1000
#endif

void LoggingTask::run() {

    vTaskDelay(
        pdMS_TO_TICKS(LOGGING_DELAY)); // Delay before actual logging starts. This is to allow the serial port to
                                       // initialize properly and to prevent the first log messages from being lost
    xSemaphoreTake(mutex_, portMAX_DELAY);
#if ENABLE_LOGGING_INIT_MESSAGE
    protocol_->log_raw(LOGGING_BOLD_TEXT LOGGING_BLUE_TEXT
                       "*******************************************" LOGGING_RESET_COLOR);
    protocol_->log_raw(LOGGING_BOLD_TEXT LOGGING_BLUE_TEXT "*   " LOGGING_CUSTOM_INIT_TITLE LOGGING_RESET_COLOR);
    protocol_->log_raw(LOGGING_BOLD_TEXT LOGGING_BLUE_TEXT
                       "*******************************************" LOGGING_RESET_COLOR);
    protocol_->log_raw("");
    protocol_->log_raw(LOGGING_GREEN_TEXT "Press 'O' to toggle origin logging" LOGGING_RESET_COLOR);
    protocol_->log_raw(LOGGING_GREEN_TEXT "Press 'V' to toggle verbose logging" LOGGING_RESET_COLOR);
    protocol_->log_raw("");
    protocol_->log_raw(LOGGING_YELLOW_TEXT "[V] = Verbose logging indicator" LOGGING_RESET_COLOR);
#if ENABLE_VERBOSE_LOGGING
#if VERBOSE_LOGGING_DEFAULT
    protocol_->log_raw(LOGGING_BOLD_TEXT "Verbose logging is currently: " LOGGING_GREEN_TEXT
                                         "ENABLED" LOGGING_RESET_COLOR);
#else
    protocol_->log_raw(LOGGING_BOLD_TEXT "Verbose logging is currently: " LOGGING_YELLOW_TEXT
                                         "DISABLED" LOGGING_RESET_COLOR);
#endif
#else
    protocol_->log_raw(LOGGING_BOLD_TEXT "Verbose logging is currently: " LOGGING_RED_TEXT
                                         "NOT SUPPORTED" LOGGING_RESET_COLOR);
#endif
    protocol_->log_raw("");
    protocol_->log_raw(LOGGING_BOLD_TEXT LOGGING_BLUE_TEXT
                       "*******************************************" LOGGING_RESET_COLOR);
    protocol_->log_raw("");
#endif
    xSemaphoreGive(mutex_);

    while (1) {

        if (xQueueReceive(log_queue, &log_msg, 0)) {
            // xSemaphoreTake(mutex_, portMAX_DELAY);
            // TODO look into why this is causing a deadlock (why is mutex_ locked?)
            protocol_->log(log_msg);
            free(log_msg);
            // xSemaphoreGive(mutex_);
        }
        // xSemaphoreTake(mutex_, portMAX_DELAY);
        protocol_->read();
        // xSemaphoreGive(mutex_);
        delay(25);
    }
}

void LoggingTask::enqueue_log(LogMessage *log_message) {
    if (xQueueSend(log_queue, &log_message, pdMS_TO_TICKS(100)) != pdTRUE) {
        // TODO handle overflow of queue, below line throws an exception but doesnt log anything useful to the end user.
        // throw std::runtime_error("Failed to send log message to queue");
        free(log_message);
    }
}

void LoggingTask::setProtocol(Protocol *protocol) {
    xSemaphoreTake(mutex_, portMAX_DELAY);
    protocol_ = protocol;
    xSemaphoreGive(mutex_);
}