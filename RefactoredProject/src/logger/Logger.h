#pragma once

#include "ILogger.h"
#include <ostream>

class Logger final : public ILogger
{
public:
    Logger() = default;

    void log(std::string_view entry) override;

private:
    std::ostream _output;
};
