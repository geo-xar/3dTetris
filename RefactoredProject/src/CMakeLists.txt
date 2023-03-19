set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(3dTetris 3dTetris.cpp)

target_link_libraries(3dTetris
    PRIVATE
      ${CONAN_LIBS}
)

add_subdirectory(main)
add_subdirectory(game)
add_subdirectory(logger)
