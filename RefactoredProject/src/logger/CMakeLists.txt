set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Logger STATIC)

target_sources(Logger
  PUBLIC
    include/ILogger.h
    include/LoggerFactory.h
  PRIVATE
    Logger.h
    Logger.cpp
    LoggerFactory.cpp
)

target_include_directories(Logger
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
