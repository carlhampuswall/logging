#pragma once
#include <FreeRTOS.h>
#include <queue.h>

#include "logging.h"
#include "protocols/protocol.h"
#include "task.h"

class LoggingTask : public Task<LoggingTask> {
  public:
    LoggingTask(const char *name, uint32_t stackDepth, UBaseType_t priority, const BaseType_t coreId = tskNO_AFFINITY,
                Protocol *protocol = nullptr)
        : Task<LoggingTask>{name, stackDepth, priority, coreId}, log_queue(xQueueCreate(10, sizeof(LogMessage))),
          protocol_(protocol) {}

    void enqueue_log(LogMessage log_message);

    void run();

  private:
    // LogAdapter *adapter_;

    Protocol *protocol_ = nullptr;

    LogMessage log_msg;
    xQueueHandle log_queue;
};