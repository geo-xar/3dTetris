#include "Logger.h"

void Logger::log(std::string_view entry)
{
    _output << entry << "\n";
}

const std::ostream& Logger::flush() const
{
    return _output;
}
