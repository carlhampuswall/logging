#pragma once
// #include "interface_callbacks.h"
// #include "motor_foc/motor_task.h"
#include "./serial_protocol.h"
// #include "uart_stream.h"
#include "logging.h"
#include <map>

typedef std::function<void(void)> DemoConfigChangeCallback;
typedef std::function<void(void)> OperationModeToggleCallback;

class SerialProtocolPlaintext : public SerialProtocol {
  public:
    using KeyCallback = std::function<void()>;

    SerialProtocolPlaintext(Stream &stream) : SerialProtocol(stream) {}
    ~SerialProtocolPlaintext() {}

    void registerKeyHandler(char key, KeyCallback callback);

    void log(LogMessage log_msg) override;
    void readSerial() override;

  private:
    std::map<char, KeyCallback> keyHandlers_;
};
