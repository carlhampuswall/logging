#pragma once
#include <Arduino.h>
#include <cstring>

#include "../log_adapter.h"

class ArduinoSerialAdapter : public LogAdapter {
  public:
    void output(const char *message) override;
    void handle_log(LogMessage log_message) override;
};