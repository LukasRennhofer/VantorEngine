# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the CHIFEngine´s TestFramework.cpp file Compilation !

cmake_minimum_required(VERSION 3.10)

# Project name
project(TestFramework)

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
    ../../Source/Graphics/chifDrawableObject.cpp
    ../../Source/Graphics/chifCloudsModel.cpp
    ../../Source/Graphics/chifSkybox.cpp
    ../../Source/Graphics/chifVolumetricClouds.cpp
    ../../Source/Graphics/chifWater.cpp
    ../../Source/Graphics/Renderer/chifBuffers.cpp
    ../../Source/Graphics/Renderer/chifBaseShader.cpp
    ../../Source/Graphics/Renderer/chifShader.cpp
    ../../Source/Graphics/Renderer/chifScreenSpaceShader.cpp
    ../../Source/Graphics/chifTerrain.cpp
    ../../Source/Graphics/chifTexture.cpp
    # platform
    ../../Source/Platform/chifInput.cpp
    ../../Source/Platform/chifWindow.cpp
    ../../Source/Platform/Platforms/chifPlatformWindows.cpp
    ../../Source/Platform/Platforms/chifPlatformLinux.cpp
    ../../Source/Platform/Platforms/chifPlatformSwitch.cpp
    # GUI
    ../../Source/GUI/chifImgui.cpp
    # Core
    ../../Source/Core/chifApplication.cpp
    ../../Source/Core/chifVersion.cpp
    ../../Source/Core/JobSystem/chifJobSystem.cpp 
    ../../Source/Core/Backlog/chifBacklog.cpp
    ../../Source/Core/Debug/chifInlineDebugger.cpp
    # Entity
    ../../Source/Entity/chifECS.cpp
    # Utils
    ../../Source/Utils/utils.cpp
    ../../Source/Utils/sdl2Utils.cpp
    ../../Source/Utils/OpenGL/glError.cpp
)

set(APP_SOURCES
    TestFramework.cpp
)

# === Platform-Specific Dependencies ===
if(PLATFORM STREQUAL "Windows")
    add_definitions(-D__WINDOWS__)  
    set(CMAKE_MAKE_PROGRAM "compToolchain/windows/make.exe")
    message(STATUS "Compiling for Windows")

    # === Create the Engine Library for Windows ===
    add_library(CHIFEngineWindows STATIC ${ENGINE_SOURCES})
    target_include_directories(CHIFEngineWindows PUBLIC Source external)

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

    target_link_libraries(CHIFEngineWindows ${WINDOWS_LIBRARIES} ${SDL2_LIBRARIES})

    # Setting up Libarie
    install(TARGETS CHIFEngineWindows DESTINATION lib)
    # install(DIRECTORY Source/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # === Create the Executable ===
    add_executable(TestFramework ${APP_SOURCES})
    # Link the engine library to the executable
    target_link_libraries(TestFramework CHIFEngineWindows)
    # Optional: Compiler warnings for the main executable
    target_compile_options(TestFramework PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Linux")
    add_definitions(-D__LINUX__)
    message(STATUS "Compiling for Linux")

    # === Create the Engine Library for Linux ===
    add_library(CHIFEngineLinux STATIC ${ENGINE_SOURCES})
    target_include_directories(CHIFEngineLinux PUBLIC Source external)

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

    target_link_libraries(CHIFEngineLinux ${LINUX_LIBRARIES} ${SDL2_LIBRARIES})

    # Setting up Libarie
    install(TARGETS CHIFEngineLinux DESTINATION lib)
    # install(DIRECTORY Source/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # === Create the Executable ===
    add_executable(TestFramework ${APP_SOURCES})
    # Link the engine library to the executable
    target_link_libraries(TestFramework CHIFEngineLinux)
    # Optional: Compiler warnings for the main executable
    target_compile_options(TestFramework PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Switch")
    add_definitions(-D__SWITCH__)
    message(STATUS "Compiling for Nintendo Switch (Homebrew)")

    # === Create the Engine Library for Linux ===
    add_library(CHIFEngineSwitch STATIC ${ENGINE_SOURCES})
    target_include_directories(CHIFEngineSwitch PUBLIC Source external)

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
    include_directories(${DEVKITPRO}/portlibs/switch/include/SDL2)

    # Assimp for Switch
    find_package(assimp REQUIRED)
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/assimp")
    include_directories(${ASSIMP_INCLUDE_DIR})

    add_definitions(-DIMGUI_IMPL_OPENGL_ES2)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)


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
    -lSDL2          # SDL2 for Switch
        ${SDL2_INCLUDE_DIRS}
    )
    target_link_libraries(CHIFEngineSwitch ${SWITCH_LIBRARIES})

    # Setting up Libarie
    install(TARGETS CHIFEngineSwitch DESTINATION lib)
    # install(DIRECTORY Source/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # === Create the Executable ===
    add_executable(TestFramework ${APP_SOURCES})
    # Link the engine library to the executable
    target_link_libraries(TestFramework CHIFEngineSwitch)
    # Optional: Compiler warnings for the main executable
    target_compile_options(TestFramework PRIVATE -Wall -Wextra)
else()
    message(FATAL_ERROR "Unknown or unsupported platform: ${PLATFORM}")
endif()