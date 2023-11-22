set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Grid)

add_library(Grid STATIC)

target_sources(Grid
  PUBLIC
    include/GridFactory.h
    include/IGrid.h
  PRIVATE
    Grid.cpp
    Grid.h
    GridFactory.cpp
)

target_include_directories(
  Grid
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(
  Grid
  PRIVATE
    FreeGLUT::freeglut_static
)
