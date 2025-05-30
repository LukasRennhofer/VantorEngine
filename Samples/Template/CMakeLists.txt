# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the Vantor´s Template Compilation !

cmake_minimum_required(VERSION 3.10)

# Project name
project(Template)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilation flags
add_compile_options(-w)

# Include directories
include_directories(../../Source/External)

# === Engine Library Sources ===
set(ENGINE_SOURCES
    # External
    ../../Source/External/imgui/imgui_draw.cpp
    ../../Source/External/imgui/imgui_demo.cpp
    ../../Source/External/imgui/imgui_impl_sdl2.cpp
    ../../Source/External/imgui/imgui_impl_opengl3.cpp
    ../../Source/External/imgui/imgui_widgets.cpp
    ../../Source/External/imgui/imgui_tables.cpp
    ../../Source/External/imgui/imgui.cpp
    ../../Source/External/stb_image_impl.cpp
    ../../Source/External/glad.c
    # graphics
    ../../Source/Graphics/vantorDrawableObject.cpp
    ../../Source/Graphics/vantorCloudsModel.cpp
    ../../Source/Graphics/vantorSkybox.cpp
    ../../Source/Graphics/vantorVolumetricClouds.cpp
    ../../Source/Graphics/vantorWater.cpp
    ../../Source/Graphics/Renderer/vantorBuffers.cpp
    ../../Source/Graphics/Renderer/vantorBaseShader.cpp
    ../../Source/Graphics/Renderer/vantorShader.cpp
    ../../Source/Graphics/Renderer/vantorScreenSpaceShader.cpp
    ../../Source/Graphics/vantorTerrain.cpp
    ../../Source/Graphics/vantorTexture.cpp
    # platform
    ../../Source/Platform/vantorWindow.cpp
    ../../Source/Platform/glError.cpp
    # GUI
    ../../Source/GUI/vantorImgui.cpp
    ../../Source/GUI/Font/vantorFont.cpp
    ../../Source/GUI/Font/vantorFontAtlas.cpp
    ../../Source/GUI/Font/vantorFontUtils.cpp
    ../../Source/GUI/Font/vantorLabel.cpp
    # Core
    ../../Source/Core/vantorVersion.cpp
    ../../Source/Core/JobSystem/vantorJobSystem.cpp 
    ../../Source/Core/Backlog/vantorBacklog.cpp
    # Utils
    ../../Source/Utils/utils.cpp
    ../../Source/Utils/sdl2Utils.cpp
)

# === Template Sources ===
set(APP_SOURCES
    Template.cpp
    )

# === Create the Engine Library ===
add_library(VantorLib STATIC ${ENGINE_SOURCES})
target_include_directories(VantorLib PUBLIC ../../Source ../../Source/external)

# === Platform-Specific Dependencies ===
if(WIN32)
    set(CMAKE_MAKE_PROGRAM "../../Source/compToolchain/windows/make.exe")
    message(STATUS "Compiling for Windows")

    set(WINDOWS_LIBRARIES
        mingw32
        SDL2
        SDL2main
        opengl32
        gdi32
        user32
        imm32
        shell32
        assimp
        freetype
    )

    find_package(assimp REQUIRED)
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}../../Source/externals/assimp")
    include_directories(${ASSIMP_INCLUDE_DIR})

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

    target_link_libraries(VantorLib ${WINDOWS_LIBRARIES} ${SDL2_LIBRARIES})

elseif(UNIX)
    message(STATUS "Compiling for Linux")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenGL REQUIRED gl)

    set(LINUX_LIBRARIES
        SDL2
        SDL2main
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
    find_package(assimp REQUIRED)
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}../../Source/externals/assimp")
    include_directories(${ASSIMP_INCLUDE_DIR})

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

    target_link_libraries(VantorLib ${LINUX_LIBRARIES} ${SDL2_LIBRARIES})

elseif(SWITCH)
    message(STATUS "Compiling for Nintendo Switch (Homebrew)")

    set(CMAKE_SYSTEM_NAME Switch)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)

    set(DEVKITPRO $ENV{DEVKITPRO})
    set(DEVKITA64 $ENV{DEVKITA64})
    set(CMAKE_C_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-gcc)
    set(CMAKE_CXX_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-g++)

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
    target_link_libraries(VantorLib ${SWITCH_LIBRARIES})
endif()

# === Create the Executable ===
add_executable(Template ${APP_SOURCES})

# Link the engine library to the executable
target_link_libraries(Template VantorLib)

# Optional: Compiler warnings for the main executable
target_compile_options(Template PRIVATE -Wall -Wextra)