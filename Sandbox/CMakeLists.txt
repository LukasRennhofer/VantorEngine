# - CMakeLists Build File for Sandbox Using Vantor -

cmake_minimum_required(VERSION 3.10)

# Set project name
project(VantorSandbox)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the path to Vantor
set(VANTOR_DIR "../Source/")

add_subdirectory(${VANTOR_DIR} vantor_build)
include_directories(${VANTOR_DIR})

# Project Source Files
set(PROJECT_SOURCES
    Application.cpp
)


add_executable(VantorSandbox ${PROJECT_SOURCES})

if(PLATFORM STREQUAL "Windows")
    message(STATUS "Compiling for Windows")
    target_link_libraries(VantorSandbox PRIVATE VantorWindows)
elseif(PLATFORM STREQUAL "Linux")
    message(STATUS "Compiling for Linux")
    # TODO
elseif(PLATFORM STREQUAL "Switch")
    message(STATUS "Compiling for Nintendo Switch")
    # TODO
endif()

target_include_directories(VantorSandbox PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(VantorSandbox PRIVATE -Wall)
install(TARGETS VantorSandbox DESTINATION bin)