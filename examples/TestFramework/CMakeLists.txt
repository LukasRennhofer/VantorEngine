# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the CHIFEngine´s main.cpp file Compilation !

cmake_minimum_required(VERSION 3.10)

# Project name
project(TestFramework)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilation flags
add_compile_options(-w)

# Include directories
include_directories(../../src/External)

# === Engine Library Sources ===
set(ENGINE_SOURCES
    ../../src/stb_image_impl.cpp
    # External
    ../../src/External/imgui/imgui_draw.cpp
    ../../src/External/imgui/imgui_demo.cpp
    ../../src/External/imgui/imgui_impl_sdl2.cpp
    ../../src/External/imgui/imgui_impl_opengl3.cpp
    ../../src/External/imgui/imgui_widgets.cpp
    ../../src/External/imgui/imgui_tables.cpp
    ../../src/External/imgui/imgui.cpp
    ../../src/External/glad.c
    # graphics
    ../../src/Graphics/chifDrawableObject.cpp
    ../../src/Graphics/chifCloudsModel.cpp
    ../../src/Graphics/chifSkybox.cpp
    ../../src/Graphics/chifVolumetricClouds.cpp
    ../../src/Graphics/chifWater.cpp
    ../../src/Graphics/Renderer/chifBuffers.cpp
    ../../src/Graphics/Renderer/chifBaseShader.cpp
    ../../src/Graphics/Renderer/chifShader.cpp
    ../../src/Graphics/Renderer/chifScreenSpaceShader.cpp
    ../../src/Graphics/chifTerrain.cpp
    ../../src/Graphics/chifTexture.cpp
    # platform
    ../../src/Platform/chifWindow.cpp
    ../../src/Platform/glError.cpp
    # GUI
    ../../src/GUI/chifImgui.cpp
    ../../src/GUI/Font/chifFont.cpp
    ../../src/GUI/Font/chifFontAtlas.cpp
    ../../src/GUI/Font/chifFontUtils.cpp
    ../../src/GUI/Font/chifLabel.cpp
    # Core
    ../../src/Core/chifVersion.cpp
    ../../src/Core/JobSystem/chifJobSystem.cpp 
    ../../src/Core/Backlog/chifBacklog.cpp
    # Utils
    ../../src/Utils/utils.cpp
    ../../src/Utils/sdl2Utils.cpp
)

# === TestFramework Sources ===
set(APP_SOURCES
    main.cpp
    )

# === Create the Engine Library ===
add_library(CHIFEngineLib STATIC ${ENGINE_SOURCES})
target_include_directories(CHIFEngineLib PUBLIC ../../src ../../src/external)

# === Platform-Specific Dependencies ===
if(WIN32)
    set(CMAKE_MAKE_PROGRAM "../../src/compToolchain/windows/make.exe")
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
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}../../src/externals/assimp")
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
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}../../src/externals/assimp")
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

    set(SWITCH_LIBRARIES
        nx
        EGL
        GLESv2
        drm_nouveau
        mesa
    )

    add_definitions(-D__SWITCH__)
    target_link_libraries(CHIFEngineLib ${SWITCH_LIBRARIES})
endif()

# === Create the Executable ===
add_executable(TestFramework ${APP_SOURCES})

# Link the engine library to the executable
target_link_libraries(TestFramework CHIFEngineLib)

# Optional: Compiler warnings for the main executable
target_compile_options(TestFramework PRIVATE -Wall -Wextra)