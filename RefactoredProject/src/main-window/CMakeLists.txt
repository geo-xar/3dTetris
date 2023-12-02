set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Main-window)

add_library(Main-window STATIC)

target_sources(Main-window
  PUBLIC
    include/IMainWindow.h
    include/MainWindowFactory.h
  PRIVATE
    MainWindow.cpp
    MainWindow.h
    MainWindowFactory.cpp
    RenderSceneCallback.cpp
    RenderSceneCallback.h
)

target_include_directories(
  Main-window
  PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(
  Main-window
  PRIVATE
    FreeGLUT::freeglut_static
)
