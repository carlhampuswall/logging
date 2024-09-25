#pragma once
#include "structs.h"

#define RESET_COLOR "\033[0m"
#define BLUE_TEXT "\033[34m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define CYAN_TEXT "\033[36m"
#define BOLD_TEXT "\033[1m"

class LogAdapter {
  public:
    LogAdapter() {};
    virtual ~LogAdapter() {};

    virtual void output(const char *message) = 0;
    virtual void handleLog(LogMessage log_message) = 0;

    virtual void setVerbose(bool verbose);
    virtual void setOrigin(bool origin);

  protected:
#if VERBOSE_LOGGING_DEFAULT
    bool verbose_ = true;
#else
    bool verbose_ = false;
#endif

    bool origin_ = false;
};