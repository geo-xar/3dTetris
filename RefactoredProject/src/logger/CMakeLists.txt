set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Logger)

add_library(Logger STATIC)

target_sources(Logger
  PUBLIC
    include/ILogger.h
    include/LoggerFactory.h
  PRIVATE
    Logger.cpp
    Logger.h
    LoggerFactory.cpp
)

target_include_directories(
  Logger
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
