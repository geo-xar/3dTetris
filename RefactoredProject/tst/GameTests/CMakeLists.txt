set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

find_package(GTest REQUIRED CONFIG)

add_executable(GameTests
    GameTests.cpp
)

target_link_libraries(
  GameTests
  PRIVATE
    Game
    Grid
    GTest::gmock
    GTest::gtest
    Logger
    Main-window
    TestUtils
)
