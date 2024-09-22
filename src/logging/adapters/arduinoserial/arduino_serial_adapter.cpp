#include "arduino_serial_adapter.h"

void ArduinoSerialAdapter::output(const char *message) { Serial.println(message); }

void ArduinoSerialAdapter::handleLog(LogMessage log_msg) {
    if (!verbose_ && log_msg.verbose) {
        return;
    } // TODO: Add something to signify verbose logging

    std::string msg = "";
    switch (log_msg.level) {
    case LOG_LEVEL_INFO:
        msg += "\u001b[37m INFO:\033[0m ";
        break;
    case LOG_LEVEL_WARNING:
        msg += "\u001b[33m WARNING:\033[0m ";
        break;
    case LOG_LEVEL_ERROR:
        msg += "\u001b[31m ERROR:\033[0m ";
        break;
    case LOG_LEVEL_DEBUG:
        msg += "\u001b[32m DEBUG:\033[0m ";
        break;
    default:
        msg += "\u001b[31m ERROR:\033[0m ";
        msg += "Unknown log level";
        output(msg.c_str());
        return;
        break;
    }

    msg += log_msg.msg;

    output(msg.c_str());
}

void ArduinoSerialAdapter::setVerbose(bool verbose) { verbose_ = verbose; }

void ArduinoSerialAdapter::setOrigin(bool origin) { origin_ = origin; }