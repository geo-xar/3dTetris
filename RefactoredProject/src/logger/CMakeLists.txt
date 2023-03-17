# Copyright 2023 by Georgios Charitos.
# All rights reserved.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(logger STATIC)

target_sources(logger
  PUBLIC
    include/ILogger.h
  PRIVATE
    Logger.h
    Logger.cpp
)

target_include_directories(logger
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
