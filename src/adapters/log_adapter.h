#pragma once

class LogAdapter {
  public:
    virtual void output(const char *message) = 0;
};