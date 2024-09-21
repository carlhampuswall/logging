#include "free_rtos_adapter.h"

FreeRTOSAdapter::FreeRTOSAdapter(Protocol *protocol, const char *name, uint32_t stackDepth, UBaseType_t priority,
                                 const BaseType_t coreId)
    : protocol_{protocol}, logging_task_{name, stackDepth, priority, coreId, protocol} {
    stream_.begin();
    logging_task_.begin();
    // log_queue = xQueueCreate(10, sizeof(LogMessage));
    // assert("Failed to create log queue" && log_queue != nullptr);
}

void FreeRTOSAdapter::output(const char *message) { Serial.println(message); }

void FreeRTOSAdapter::handle_log(LogMessage log_msg) {

    // if (!verbose_ && log_message.verbose) {
    //     // TODO: Add verbose logging
    //     return;
    // }
    // std::string msg = "";
    // switch (log_message.level) {
    // case LOG_LEVEL_INFO:
    //     msg += "\u001b[37m INFO:\033[0m ";
    //     break;
    // case LOG_LEVEL_WARNING:
    //     msg += "\u001b[33m WARNING:\033[0m ";
    //     break;
    // case LOG_LEVEL_ERROR:
    //     msg += "\u001b[31m ERROR:\033[0m ";
    //     break;
    // case LOG_LEVEL_DEBUG:
    //     msg += "\u001b[32m DEBUG:\033[0m ";
    //     break;
    // default:
    //     msg += "\u001b[31m ERROR:\033[0m ";
    //     msg += "Unknown log level";
    //     output(msg.c_str());
    //     return;
    //     break;
    // }

    // msg += log_message.msg;

    logging_task_.enqueue_log(log_msg);

    // output(msg.c_str());
}

void FreeRTOSAdapter::setVerbose(bool verbose) { protocol_->setVerbose(verbose); }

void FreeRTOSAdapter::setOrigin(bool origin) { protocol_->setOrigin(origin); }