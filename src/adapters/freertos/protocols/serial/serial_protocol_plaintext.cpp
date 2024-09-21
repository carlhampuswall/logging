#include "serial_protocol_plaintext.h"

void SerialProtocolPlaintext::log(LogMessage log_msg) {
    if (!verbose_ && log_msg.verbose) {
        return;
    }

    if (origin_) {
        stream_.printf("[%s]", log_msg.origin);
    }

    switch (log_msg.level) {
    case LOG_LEVEL_INFO:
        stream_.print("\u001b[37m INFO:\033[0m ");
        break;
    case LOG_LEVEL_WARNING:
        stream_.print("\u001b[33m WARNING:\033[0m ");
        break;
    case LOG_LEVEL_ERROR:
        stream_.print("\u001b[31m ERROR:\033[0m ");
        break;
    case LOG_LEVEL_DEBUG:
        stream_.print("\u001b[32m DEBUG:\033[0m ");
        break;
    default:
        stream_.print("\u001b[37m  INFO:\033[0m ");
        break;
    }
    stream_.println(log_msg.msg);
    stream_.flush();
}