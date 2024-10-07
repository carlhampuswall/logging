#include "free_rtos_adapter.h"

FreeRTOSAdapter::FreeRTOSAdapter(Protocol *protocol, SemaphoreHandle_t mutex, const char *name, uint32_t stackDepth,
                                 UBaseType_t priority, const BaseType_t coreId)
    : protocol_{protocol}, mutex_(mutex), logging_task_{
                                              mutex, protocol, name, stackDepth, priority, coreId,
                                          } {
    logging_task_.begin();
}

void FreeRTOSAdapter::output(const char *message) {
    // Serial.println(message);
}

void FreeRTOSAdapter::handleLog(LogMessage log_msg) {
    xSemaphoreTake(mutex_, portMAX_DELAY);
    logging_task_.enqueue_log(log_msg);
    xSemaphoreGive(mutex_);
}

void FreeRTOSAdapter::setVerbose(bool verbose) {
    xSemaphoreTake(mutex_, portMAX_DELAY);
    protocol_->setVerbose(verbose);
    xSemaphoreGive(mutex_);
}

void FreeRTOSAdapter::setOrigin(bool origin) {
    xSemaphoreTake(mutex_, portMAX_DELAY);
    protocol_->setOrigin(origin);
    xSemaphoreGive(mutex_);
}

void FreeRTOSAdapter::setProtocol(Protocol *protocol) {
    xSemaphoreTake(mutex_, portMAX_DELAY);
    protocol_ = protocol;
    xSemaphoreGive(mutex_);
    logging_task_.setProtocol(protocol);
}

Protocol *FreeRTOSAdapter::getProtocol() { return protocol_; }

bool FreeRTOSAdapter::available() {
    TaskHandle_t taskHandle = logging_task_.getHandle();
    if (taskHandle == nullptr) {
        return false;
    }
    eTaskState taskState = eTaskGetState(taskHandle);
    return taskState == eRunning || taskState == eReady;
}