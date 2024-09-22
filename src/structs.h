#pragma once
enum LOG_LEVEL { LOG_LEVEL_INFO = 1, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_DEBUG, LOG_LEVEL_COUNT };

#ifndef ORIGIN_MAX_SIZE
#define ORIGIN_MAX_SIZE 256
#endif

#ifndef MSG_MAX_SIZE
#define MSG_MAX_SIZE 1024
#endif

struct LogMessage {
    LOG_LEVEL level;
    bool verbose;
    unsigned long timestamp;
    char origin[ORIGIN_MAX_SIZE];
    char msg[MSG_MAX_SIZE];
};