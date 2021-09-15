cmake_minimum_required(VERSION 3.20)

project(RTMP VERSION 1.0)

# Set c++ 17 standard for msvc 17.
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

# Source files
set(SOURCE 
    test/win32_server.cpp
    src/core/RTMPMessage.cpp
    src/core/RTMPParser.cpp
    src/core/RTMPHandler.cpp
)

set (TEST_SOURCE
    test/debug/test_handshake.cpp
    src/core/RTMPParser.cpp
    src/core/RTMPHandler.cpp
)

add_executable(server ${SOURCE})
add_executable(test_handshake ${TEST_SOURCE})