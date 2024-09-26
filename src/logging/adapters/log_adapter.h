#pragma once
#include "structs.h"

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