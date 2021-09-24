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

add_subdirectory(src)
add_subdirectory(libs)

add_executable(server test/win32_server.cpp)
add_executable(decoder test/test_decoder.cpp)

target_include_directories(decoder PUBLIC "src/core/" "src/utils/" "src/codecs" "includes/")
target_link_libraries(decoder libs)
target_link_libraries(decoder rtmp_lib)

target_include_directories(server PUBLIC "includes/")
target_include_directories(server PUBLIC "src/utils/")
target_link_libraries(server libs)
target_link_libraries(server rtmp_lib)