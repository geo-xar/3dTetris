#pragma once

#include <string_view>
#include <memory>
#include <ostream>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void log(std::string_view entry) = 0;
    virtual const std::ostream& flush() const = 0;
};

using ILoggerPtrU = std::unique_ptr<ILogger>;
