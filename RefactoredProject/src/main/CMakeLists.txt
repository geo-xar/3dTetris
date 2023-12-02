set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(New3dTetris)

add_executable(New3dTetris main.cpp)

target_link_libraries(
  New3dTetris
  PRIVATE
    Game
    Grid
    Logger
    Main-window
)
