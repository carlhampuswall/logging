#pragma once

#include <functional>

// #include "logging/logger_task.h"
#include "adapters/freertos/protocols/protocol.h"

#define SERIAL_PROTOCOL_LEGACY 0
#define SERIAL_PROTOCOL_PROTO 1

// typedef std::function<void(uint8_t)> ProtocolChangeCallback;

class SerialProtocol : public Protocol {
  public:
    SerialProtocol(Stream &stream) : Protocol(), stream_(stream) {}
    virtual ~SerialProtocol() {}

  protected:
    Stream &stream_;
};
