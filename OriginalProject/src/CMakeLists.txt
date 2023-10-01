set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

find_package(FreeGLUT REQUIRED CONFIG)

add_executable(3dTetris 3dTetris.cpp)

target_link_libraries(3dTetris
    PRIVATE
      FreeGLUT::freeglut_static
)
