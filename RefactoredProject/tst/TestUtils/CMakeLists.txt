set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(TestUtils INTERFACE)

target_sources(TestUtils
    INTERFACE
      LoggerMock.h
)

target_include_directories(TestUtils
    INTERFACE
      "${CMAKE_CURRENT_LIST_DIR}"
)
