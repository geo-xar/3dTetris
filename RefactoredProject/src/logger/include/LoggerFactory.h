#pragma once

#include "ILogger.h"

class LoggerFactory final
{
public:
    static ILoggerPtrU createLogger();
};
