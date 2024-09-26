#pragma once
enum LOG_LEVEL { LOG_LEVEL_INFO = 1, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_DEBUG, LOG_LEVEL_COUNT };

#ifndef ORIGIN_MAX_SIZE
#define ORIGIN_MAX_SIZE 256
#endif

#ifndef MSG_MAX_SIZE
#define MSG_MAX_SIZE 1024
#endif

#define LOGGING_RESET_COLOR "\033[0m"
#define LOGGING_RED_TEXT "\033[31m"
#define LOGGING_BLUE_TEXT "\033[34m"
#define LOGGING_GREEN_TEXT "\033[32m"
#define LOGGING_YELLOW_TEXT "\033[33m"
#define LOGGING_CYAN_TEXT "\033[36m"
#define LOGGING_GRAY_TEXT "\033[37m"
#define LOGGING_BOLD_TEXT "\033[1m"

struct LogMessage {
    LOG_LEVEL level;
    bool verbose;
    unsigned long timestamp;
    char origin[ORIGIN_MAX_SIZE];
    char msg[MSG_MAX_SIZE];
};