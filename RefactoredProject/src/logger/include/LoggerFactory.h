#pragma once

#include "ILogger.h"

class LoggerFactory final
{
public:
    [[nodiscard]] static ILoggerPtrU createLogger();
};
