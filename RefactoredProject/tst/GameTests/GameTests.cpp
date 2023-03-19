#include "gtest/gtest.h"
#include "LoggerMock.h"
#include "Game.h"

class GameTests : public ::testing::Test
{
public:
    GameTests()
    : _loggerMock{std::make_unique<LoggerMock>()}
    {}

    LoggerMockPtrU _loggerMock;
};

TEST_F(GameTests, firstGameTest)
{
    Game game{ *_loggerMock };
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
