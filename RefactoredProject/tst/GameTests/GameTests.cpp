#include "Game.h"
#include "GridMock.h"
#include "gtest/gtest.h"
#include "LoggerMock.h"
#include "MainWindowMock.h"

class GameTests : public ::testing::Test
{
public:
    GameTests()
    : _loggerMock{std::make_unique<LoggerMock>()}
    , _gridMock{std::make_unique<GridMock>()}
    , _mainWindowMock{std::make_unique<MainWindowMock>()}
    {}

    LoggerMockPtrU _loggerMock;
    GridMockPtrU _gridMock;
    MainWindowMockPtrU _mainWindowMock;
};

TEST_F(GameTests, firstGameTest)
{
    Game game{ *_loggerMock, *_gridMock, *_mainWindowMock };
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
