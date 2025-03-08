cmake_minimum_required(VERSION 3.10)
project(CHIFEngine)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define paths
set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/src/Libraries)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/Include)

# Glad
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/src/Compile/glad.c)

# Include directories
include_directories(${INCLUDE_DIR})

# Link directories
link_directories(${LIBRARY_DIR})

# Source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw3 opengl32 gdi32)
