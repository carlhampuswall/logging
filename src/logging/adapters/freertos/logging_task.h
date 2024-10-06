#pragma once
#include <FreeRTOS.h>
#include <queue.h>

#include "logging.h"
#include "protocols/protocol.h"
#include "task.h"

class LoggingTask : public FreeRTOS::Task<LoggingTask> {
  public:
    LoggingTask(SemaphoreHandle_t mutex, Protocol *protocol, const char *name, uint32_t stackDepth,
                UBaseType_t priority, const BaseType_t coreId = tskNO_AFFINITY)
        : FreeRTOS::Task<LoggingTask>{name, stackDepth, priority, coreId},
          log_queue(xQueueCreate(10, sizeof(LogMessage))), protocol_(protocol), mutex_(mutex) {}

    void enqueue_log(const LogMessage &log_message);

    void run();

    void setProtocol(Protocol *protocol);

  private:
    // LogAdapter *adapter_;
    Protocol *protocol_;
    SemaphoreHandle_t mutex_;

    LogMessage log_msg;
    xQueueHandle log_queue;
};