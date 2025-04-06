# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the CHIFEngine´s main.cpp file Compilation !

cmake_minimum_required(VERSION 3.10)

# Project name
project(CHIFEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilation flags
add_compile_options(-w)

# Include directories
include_directories(External)

# === Engine Library Sources ===
set(ENGINE_SOURCES
    stb_image_impl.cpp
    # External
    External/imgui/imgui_draw.cpp
    External/imgui/imgui_demo.cpp
    External/imgui/imgui_impl_sdl2.cpp
    External/imgui/imgui_impl_opengl3.cpp
    External/imgui/imgui_widgets.cpp
    External/imgui/imgui_tables.cpp
    External/imgui/imgui.cpp
    External/glad.c
    # graphics
    Graphics/chifDrawableObject.cpp
    Graphics/chifCloudsModel.cpp
    Graphics/chifSkybox.cpp
    Graphics/chifVolumetricClouds.cpp
    Graphics/chifWater.cpp
    Graphics/Renderer/chifBuffers.cpp
    Graphics/Renderer/chifBaseShader.cpp
    Graphics/Renderer/chifShader.cpp
    Graphics/Renderer/chifScreenSpaceShader.cpp
    Graphics/chifTerrain.cpp
    Graphics/chifTexture.cpp
    # platform
    Platform/chifWindow.cpp
    Platform/glError.cpp
    # GUI
    GUI/Font/chifFont.cpp
    GUI/Font/chifFontAtlas.cpp
    GUI/Font/chifFontUtils.cpp
    GUI/Font/chifLabel.cpp
    # Core
    Core/chifVersion.cpp
    Core/JobSystem/chifJobSystem.cpp 
    Core/Backlog/chifBacklog.cpp
    Core/Debug/chifInlineDebugger.cpp
    # Utils
    Utils/utils.cpp
    Utils/sdl2Utils.cpp
)

# === Create the Engine Library ===
add_library(CHIFEngineLib STATIC ${ENGINE_SOURCES})
target_include_directories(CHIFEngineLib PUBLIC src external)

# === Platform-Specific Dependencies ===
if(WIN32)
    set(CMAKE_MAKE_PROGRAM "compToolchain/windows/make.exe")
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
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/assimp")
    include_directories(${ASSIMP_INCLUDE_DIR})

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

    target_link_libraries(CHIFEngineLib ${WINDOWS_LIBRARIES} ${SDL2_LIBRARIES})

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
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/assimp")
    include_directories(${ASSIMP_INCLUDE_DIR})

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

    target_link_libraries(CHIFEngineLib ${LINUX_LIBRARIES} ${SDL2_LIBRARIES})

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

    # Link SDL2 from DevKitPro
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})

    # Assimp for Switch
    find_package(assimp REQUIRED)
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/assimp")
    include_directories(${ASSIMP_INCLUDE_DIR})

    # Include FreeType for Switch
    find_package(Freetype REQUIRED)
    include_directories(${FREETYPE_INCLUDE_DIRS})

    set(SWITCH_LIBRARIES
    nx             # Nintendo Switch SDK
    GLESv2         # OpenGL ES 2.0 for Switch
    drm_nouveau    # For graphics on Switch
    mesa           # OpenGL ES implementation
    assimp         # Assimp library for Switch
    freetype       # Freetype for Switch
    SDL2           # SDL2 for Switch
        ${SDL2_INCLUDE_DIRS}
    )

    add_definitions(-D__SWITCH__)
    target_link_libraries(CHIFEngineLib ${SWITCH_LIBRARIES})
endif()

# === Create the Executable ===
add_executable(CHIFEngine main.cpp)

# Link the engine library to the executable
target_link_libraries(CHIFEngine CHIFEngineLib)

# Optional: Compiler warnings for the main executable
target_compile_options(CHIFEngine PRIVATE -Wall -Wextra)