#pragma once
#include <Arduino.h>
#include <cstring>

// #include "../log_adapter.h"
#include "logging_task.h"
#include "protocols/protocol.h"
#include "protocols/serial/serial_protocol_plaintext.h"
#include "task.h"

class FreeRTOSAdapter : public LogAdapter {
  public:
    FreeRTOSAdapter(Protocol *protocol, const char *name, uint32_t stackDepth, UBaseType_t priority,
                    const BaseType_t coreId = 0);

    void setProtocol(Protocol *protocol);

    void output(const char *message) override;
    void handleLog(LogMessage log_message) override;

    void setVerbose(bool verbose) override;
    void setOrigin(bool origin) override;

  private:
    LoggingTask logging_task_;
    Protocol *protocol_ = nullptr;

    SemaphoreHandle_t mutex_;

#if defined(CONFIG_IDF_TARGET_ESP32S3) && !SK_FORCE_UART_STREAM
    HWCDC stream_;
#else
    UartStream stream_;
#endif
};