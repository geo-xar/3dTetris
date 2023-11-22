set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(LoggerTests)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

find_package(GTest REQUIRED CONFIG)

add_executable(LoggerTests LoggerTests.cpp)

target_link_libraries(
  LoggerTests
  PRIVATE
    Logger
    GTest::gtest
)
