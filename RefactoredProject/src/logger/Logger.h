#pragma once

#include "ILogger.h"
#include <sstream>

class Logger final : public ILogger
{
public:
    void log(std::string_view entry) override;
    const std::ostream& flush() const override;

private:
    std::stringstream _output{};
};
