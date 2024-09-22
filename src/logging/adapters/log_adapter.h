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
    bool verbose_ = false;
    bool origin_ = false;
};