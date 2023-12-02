set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Game)

add_library(Game STATIC)

target_sources(Game
  PUBLIC
    include/Game.h
  PRIVATE
    Game.cpp
)

target_include_directories(
  Game
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(
  Game
  PRIVATE
    Grid
    Logger
    Main-window
)
