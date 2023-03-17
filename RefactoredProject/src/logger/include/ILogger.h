#pragma once

#include <string_view>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void log(std::string_view entry) = 0;
};
