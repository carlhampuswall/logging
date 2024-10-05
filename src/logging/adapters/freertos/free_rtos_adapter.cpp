#include "free_rtos_adapter.h"

FreeRTOSAdapter::FreeRTOSAdapter(Protocol *protocol, const char *name, uint32_t stackDepth, UBaseType_t priority,
                                 const BaseType_t coreId)
    : protocol_{protocol}, logging_task_{
                               mutex_, protocol, name, stackDepth, priority, coreId,
                           } {
    mutex_ = xSemaphoreCreateMutex();

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
}

bool FreeRTOSAdapter::available() {
    TaskHandle_t taskHandle = logging_task_.getHandle();
    if (taskHandle == nullptr) {
        return false;
    }
    eTaskState taskState = eTaskGetState(taskHandle);
    return taskState == eRunning || taskState == eReady;
}