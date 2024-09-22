#include "free_rtos_adapter.h"

FreeRTOSAdapter::FreeRTOSAdapter(Protocol *protocol, const char *name, uint32_t stackDepth, UBaseType_t priority,
                                 const BaseType_t coreId)
    : protocol_{protocol}, logging_task_{
                               mutex_, protocol, name, stackDepth, priority, coreId,
                           } {
    mutex_ = xSemaphoreCreateMutex();

    stream_.begin();
    logging_task_.begin();
}

void FreeRTOSAdapter::output(const char *message) {
    // Serial.println(message);
}

void FreeRTOSAdapter::handleLog(LogMessage log_msg) { logging_task_.enqueue_log(log_msg); }

void FreeRTOSAdapter::setVerbose(bool verbose) { protocol_->setVerbose(verbose); }

void FreeRTOSAdapter::setOrigin(bool origin) { protocol_->setOrigin(origin); }

void FreeRTOSAdapter::setProtocol(Protocol *protocol) {
    xSemaphoreTake(mutex_, portMAX_DELAY);
    protocol_ = protocol;
    xSemaphoreGive(mutex_);
}