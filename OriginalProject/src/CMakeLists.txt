# Copyright 2023 by Georgios Charitos.
# All rights reserved.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(GLUT 3.2.2 REQUIRED)

add_executable(3dTetris 3dTetris.cpp)

target_link_libraries(3dTetris
    PRIVATE
      GLUT::GLUT
)
