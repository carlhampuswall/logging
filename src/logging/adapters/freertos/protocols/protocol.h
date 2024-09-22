#pragma once
#include "../task.h"
#include "structs.h"

class Protocol {
  public:
    Protocol() {};
    ~Protocol() {};
    virtual void log(LogMessage log_msg) = 0;
    virtual void read() = 0;

    // bool isVerbose() { return verbose; }
    void setVerbose(bool verbose) { verbose_ = verbose; }

    // bool logOrigin() { return log_origin; }
    void setOrigin(bool origin) { origin_ = origin; }

  protected:
    bool verbose_ = false;
    bool origin_ = false;
};