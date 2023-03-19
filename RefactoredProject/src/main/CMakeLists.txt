set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(new3dtetris main.cpp)

target_link_libraries(new3dtetris
  PRIVATE
    logger
    game
)
