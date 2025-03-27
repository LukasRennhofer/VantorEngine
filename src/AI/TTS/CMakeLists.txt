# cmake needs this line
cmake_minimum_required(VERSION 3.10)

# Define project name
project(vits)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "v")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find ONNX Runtime
find_path(ONNX_RUNTIME_SESSION_INCLUDE_DIRS onnxruntime_cxx_api.h HINTS /home/lrdev/onnxruntime/onnxruntime-linux-x64-1.17.0/include)
find_library(ONNX_RUNTIME_LIB onnxruntime HINTS /home/lrdev/onnxruntime/onnxruntime-linux-x64-1.17.0/lib)

# Find espeak-ng
find_library(ESPEAK_LIB espeak-ng REQUIRED)

if (ESPEAK_LIB)
    message(STATUS "Found espeak-ng: ${ESPEAK_LIB}")
else()
    message(FATAL_ERROR "espeak-ng library not found!")
endif()

# Include project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/main.cpp" "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.c" "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp")

# Create executable (only once)
add_executable(vits ${SOURCES})

# Link libraries
target_include_directories(vits PRIVATE ${ONNX_RUNTIME_SESSION_INCLUDE_DIRS})
target_link_libraries(vits PRIVATE ${ONNX_RUNTIME_LIB} ${ESPEAK_LIB})
