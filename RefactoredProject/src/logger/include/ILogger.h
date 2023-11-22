#pragma once

#include <memory>
#include <ostream>
#include <string_view>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void log(std::string_view entry) = 0;
    virtual const std::ostream& flush() const = 0;
};

using ILoggerPtrU = std::unique_ptr<ILogger>;
