cmake_minimum_required(VERSION 3.10)

# Project name
project(CHIFEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(
    src/Include
    src/imgui
    src/Compile
)

# Add source files
set(SOURCES
    src/Engine/Window.cpp
    src/Engine/utils.cpp
    src/Engine/texture.cpp
    src/Engine/shader.cpp
    src/Engine/ScreenSpaceShader.cpp
    src/Engine/glError.cpp
    src/Engine/buffers.cpp
    src/Engine/BaseShader.cpp
    src/Engine/version.cpp
    src/DrawableObjects/Water.cpp
    src/DrawableObjects/VolumetricClouds.cpp
    src/DrawableObjects/Terrain.cpp
    src/DrawableObjects/Skybox.cpp
    src/DrawableObjects/GUI.cpp
    src/DrawableObjects/drawableObject.cpp
    src/DrawableObjects/CloudsModel.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_impl_opengl3.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/main.cpp
    src/Compile/glad.c
)

# Platform-specific handling
if(WIN32)
    set(CMAKE_MAKE_PROGRAM "compToolchain/windows/make.exe")
    # Windows-specific setup
    message(STATUS "Compiling for Windows")
    set(WINDOWS_LIBRARIES
        glfw3
        opengl32
        gdi32
        user32
        imm32
        shell32
    )

    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/src/Libraries")
    set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/Include/GLFW")

    include_directories(${GLFW_INCLUDE_DIR})
    link_directories(${GLFW_LIB_DIR})



elseif(UNIX)
    # Linux-specific setup
    message(STATUS "Compiling for Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED glfw3)
    pkg_check_modules(OpenGL REQUIRED gl)

    set(LINUX_LIBRARIES
        ${GLFW3_LIBRARIES}
        ${OpenGL_LIBRARIES}
        X11
        pthread
        dl
        jsoncpp
        fmt
        assimp
        EGL
        GLESv2
        gbm
        drm
    )

elseif(SWITCH)
    # Switch-specific setup
    message(STATUS "Compiling for Nintendo Switch (Homebrew)")

    set(CMAKE_SYSTEM_NAME Switch)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)

    # Set toolchain paths
    set(DEVKITPRO $ENV{DEVKITPRO})
    set(DEVKITA64 $ENV{DEVKITA64})
    set(CMAKE_C_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-gcc)
    set(CMAKE_CXX_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-g++)

    # Include directories for Switch
    include_directories(
        ${DEVKITPRO}/libnx/include
        ${DEVKITPRO}/portlibs/switch/include
    )

    link_directories(
        ${DEVKITPRO}/libnx/lib
        ${DEVKITPRO}/portlibs/switch/lib
    )

    set(SWITCH_LIBRARIES
        nx
        EGL
        GLESv2
        drm_nouveau
        mesa
    )

    add_definitions(-D__SWITCH__)
endif()

# Create the executable
add_executable(CHIFEngine ${SOURCES} src/stb_image_impl.cpp)

# Link platform-specific libraries
target_link_libraries(CHIFEngine ${LINUX_LIBRARIES} ${WINDOWS_LIBRARIES} ${SWITCH_LIBRARIES})
target_compile_options(CHIFEngine PRIVATE -Wall -Wextra)
