#pragma once
// #include "interface_callbacks.h"
// #include "motor_foc/motor_task.h"
#include "./serial_protocol.h"
// #include "uart_stream.h"

typedef std::function<void(void)> DemoConfigChangeCallback;
typedef std::function<void(void)> OperationModeToggleCallback;

class SerialProtocolPlaintext : public SerialProtocol {
  public:
    SerialProtocolPlaintext(Stream &stream) : SerialProtocol(stream) {}
    ~SerialProtocolPlaintext() {}
    void log(LogMessage log_msg) override;
};
