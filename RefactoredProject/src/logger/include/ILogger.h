#pragma once

#include <string_view>
#include <memory>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void log(std::string_view entry) = 0;
};

using ILoggerPtrU = std::unique_ptr<ILogger>;
