cmake_minimum_required(VERSION 3.10)

# Define platform (must match logic in Vantor's CMakeLists)
# set(PLATFORM "Linux") # or "Windows" or "Switch"

# Include CMake build files
include(../Vantor/CMake/VantorCMake.cmake)

project(VantorSandbox)
set(CMAKE_CXX_STANDARD 20)

# Add engine subdirectory
add_subdirectory(../Vantor/Source ${CMAKE_BINARY_DIR}/VantorBuild)

# Add Source Code of project
add_executable(VantorSandbox Application.cpp)

# Set up Project Target for Vantor
SetUpVantorTarget(VantorSandbox)

# Include Engine API Includes
target_include_directories(VantorSandbox PRIVATE ../Vantor/Include)

# Link with Vantor
target_link_libraries(VantorSandbox PRIVATE Vantor)