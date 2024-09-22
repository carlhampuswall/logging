#pragma once
enum LOG_LEVEL { LOG_LEVEL_INFO = 1, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_DEBUG, LOG_LEVEL_COUNT };

#define ORIGIN_MAX_SIZE 256
#define MSG_MAX_SIZE 1024

struct LogMessage {
    LOG_LEVEL level;
    bool verbose;
    unsigned long timestamp;
    // std::string origin;
    // std::string msg;
    char origin[ORIGIN_MAX_SIZE];
    char msg[MSG_MAX_SIZE];
};