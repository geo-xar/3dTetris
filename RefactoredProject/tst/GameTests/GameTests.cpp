#include "Game.h"
#include "gtest/gtest.h"
#include "LoggerMock.h"
#include "GridMock.h"

class GameTests : public ::testing::Test
{
public:
    GameTests()
    : _loggerMock{std::make_unique<LoggerMock>()}
    , _gridMock{std::make_unique<GridMock>()}
    {}

    LoggerMockPtrU _loggerMock;
    GridMockPtrU _gridMock;
};

TEST_F(GameTests, firstGameTest)
{
    Game game{ *_loggerMock, *_gridMock };
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
