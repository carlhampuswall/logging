#pragma once
enum LOG_LEVEL { LOG_LEVEL_INFO = 1, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_DEBUG, LOG_LEVEL_COUNT };

struct LogMessage {
    LOG_LEVEL level;
    bool verbose;
    unsigned long timestamp;
    const char *origin;
    const char *msg;
};
