cmake_minimum_required(VERSION 3.10)

# Project name
project(CHIFEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(src/external)

# Add source files
set(SOURCES
    # src
    src/main.cpp
    src/stb_image_impl.cpp
    # Engine
    src/Engine/buffers.cpp
    src/Engine/BaseShader.cpp
    src/Engine/shader.cpp
    src/Engine/ScreenSpaceShader.cpp
    # externals
    src/external/imgui/imgui_draw.cpp
    src/external/imgui/imgui_demo.cpp
    src/external/imgui/imgui_impl_glfw.cpp
    src/external/imgui/imgui_impl_opengl3.cpp
    src/external/imgui/imgui_widgets.cpp
    src/external/imgui/imgui.cpp
    src/external/glad.c
    # graphics
    src/graphics/drawableObject.cpp
    src/graphics/CloudsModel.cpp
    src/graphics/Skybox.cpp
    src/graphics/VolumetricClouds.cpp
    src/graphics/Water.cpp
    # platform
    src/platform/Window.cpp
    src/platform/glError.cpp
    # gui
    src/gui/ImguiGUI.cpp
    # core
    src/core/version.cpp
    # utilities
    src/utilities/utils.cpp
    src/utilities/terrain/Terrain.cpp
    src/utilities/texture/texture.cpp
)

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
        assimp
    )

    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/src/lib")
    set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/externals/GLFW")

    include_directories(${GLFW_INCLUDE_DIR})
    link_directories(${GLFW_LIB_DIR})

    set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/src/lib")
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/externals/assimp")

    include_directories(${ASSIMP_INCLUDE_DIR})
    link_directories(${ASSIMP_LIB})
    


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
if(WIN32)
    target_link_libraries(CHIFEngine ${WINDOWS_LIBRARIES})
elseif(UNIX)
    target_link_libraries(CHIFEngine ${LINUX_LIBRARIES})
elseif(SWITCH)
    target_link_libraries(CHIFEngine ${SWITCH_LIBRARIES})
endif()

target_compile_options(CHIFEngine PRIVATE -Wall -Wextra)
