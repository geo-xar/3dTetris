#include "LoggerFactory.h"
#include "Logger.h"

ILoggerPtrU LoggerFactory::createLogger()
{
    return std::make_unique<Logger>();
}
