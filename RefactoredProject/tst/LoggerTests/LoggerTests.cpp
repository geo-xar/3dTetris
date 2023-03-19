#include "gtest/gtest.h"
#include "ILogger.h"
#include "LoggerFactory.h"

class LoggerTests : public ::testing::Test
{
public:
    LoggerTests()
    {
        _logger = LoggerFactory::createLogger();
    }

    ILoggerPtrU _logger;
};

TEST_F(LoggerTests, firstLoggerTest)
{
    const std::string logEntry{ "Hello World" };
    _logger->log(logEntry);

    const std::string actualLogEntry((std::istreambuf_iterator<char>(_logger->flush().rdbuf())),
        std::istreambuf_iterator<char>());

    EXPECT_EQ(actualLogEntry, logEntry + "\n");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
