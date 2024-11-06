#pragma once
#include <Arduino.h>
#include <cstring>

#include "../log_adapter.h"

class ArduinoSerialAdapter : public LogAdapter {
  public:
    void output(const char *message) override;
    void handleLog(LogMessage *log_message) override;

    void setVerbose(bool verbose) override;
    void setOrigin(bool origin) override;

    bool available() override { return true; } // TODO Implement
};