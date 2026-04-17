#pragma once

#include <string>

class IStatusReporter {
public:
    virtual ~IStatusReporter() = default;
    virtual void PrintStatus(const char *msg) = 0;
    virtual void PrintStatus(std::string msg) = 0;
    virtual bool ShouldAbortWork() const { return false; }
};
