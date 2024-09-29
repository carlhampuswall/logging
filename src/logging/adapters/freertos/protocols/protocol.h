#pragma once
#include "../task.h"
#include "logging.h"
#include "structs.h"

class Protocol {
  public:
    Protocol() {};
    ~Protocol() {};
    virtual void log(const LogMessage &log_msg) = 0;
    virtual void log_raw(const char *msg) = 0;
    virtual void read() = 0;
    virtual bool isAvailable() = 0;

    void setVerbose(bool verbose) { verbose_ = verbose; }

    void setOrigin(bool origin) { origin_ = origin; }

  protected:
#if VERBOSE_LOGGING_DEFAULT
    bool verbose_ = true;
#else
    bool verbose_ = false;
#endif
    bool origin_ = false;
};