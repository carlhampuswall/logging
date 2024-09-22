#pragma once
#include "../task.h"
#include "logging.h"
#include "structs.h"

class Protocol {
  public:
    Protocol() {};
    ~Protocol() {};
    virtual void log(const LogMessage &log_msg) = 0;
    virtual void read() = 0;

    void setVerbose(bool verbose) { verbose_ = verbose; }

    void setOrigin(bool origin) { origin_ = origin; }

  protected:
    bool verbose_ = false;
    bool origin_ = false;
};