#pragma once

#include "ILogger.h"
#include "gmock/gmock.h"

class LoggerMock : public ILogger
{
public:
    MOCK_METHOD1(log, void(std::string_view));
    MOCK_CONST_METHOD0(flush, std::ostream& (void));
};

using LoggerMockPtrU = std::unique_ptr<LoggerMock>;
