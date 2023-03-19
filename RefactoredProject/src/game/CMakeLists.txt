set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(game STATIC)

target_sources(game
  PUBLIC
    include/Game.h
  PRIVATE
    Game.cpp
)

target_include_directories(game
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(game
  PRIVATE
    logger
)
