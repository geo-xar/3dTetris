set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(LoggerTests
    LoggerTests.cpp
)

target_link_libraries(LoggerTests
    PRIVATE
      logger
      ${CONAN_LIBS}
)
