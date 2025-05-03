# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the Vantor´s TestFramework.cpp file Compilation !

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
    ../../Source/Platform/vantorInput.cpp
    ../../Source/Platform/vantorWindow.cpp
    ../../Source/Platform/Platforms/vantorPlatformWindows.cpp
    ../../Source/Platform/Platforms/vantorPlatformLinux.cpp
    ../../Source/Platform/Platforms/vantorPlatformSwitch.cpp
    # GUI
    ../../Source/GUI/vantorImgui.cpp
    # Core
    ../../Source/Core/vantorApplication.cpp
    ../../Source/Core/vantorVersion.cpp
    ../../Source/Core/JobSystem/vantorJobSystem.cpp 
    ../../Source/Core/Backlog/vantorBacklog.cpp
    ../../Source/Core/Debug/vantorInlineDebugger.cpp
    # Entity
    ../../Source/Entity/vantorECS.cpp
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
    add_library(VantorWindows STATIC ${ENGINE_SOURCES})
    target_include_directories(VantorWindows PUBLIC Source external)

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

    target_link_libraries(VantorWindows ${WINDOWS_LIBRARIES} ${SDL2_LIBRARIES})

    # Setting up Libarie
    install(TARGETS VantorWindows DESTINATION lib)
    # install(DIRECTORY Source/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # === Create the Executable ===
    add_executable(TestFramework ${APP_SOURCES})
    # Link the engine library to the executable
    target_link_libraries(TestFramework VantorWindows)
    # Optional: Compiler warnings for the main executable
    target_compile_options(TestFramework PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Linux")
    add_definitions(-D__LINUX__)
    message(STATUS "Compiling for Linux")

    # === Create the Engine Library for Linux ===
    add_library(VantorLinux STATIC ${ENGINE_SOURCES})
    target_include_directories(VantorLinux PUBLIC Source external)

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

    target_link_libraries(VantorLinux ${LINUX_LIBRARIES} ${SDL2_LIBRARIES})

    # Setting up Libarie
    install(TARGETS VantorLinux DESTINATION lib)
    # install(DIRECTORY Source/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # === Create the Executable ===
    add_executable(TestFramework ${APP_SOURCES})
    # Link the engine library to the executable
    target_link_libraries(TestFramework VantorLinux)
    # Optional: Compiler warnings for the main executable
    target_compile_options(TestFramework PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Switch")
    add_definitions(-D__SWITCH__)
    message(STATUS "Compiling for Nintendo Switch (Homebrew)")

    # === Create the Engine Library for Linux ===
    add_library(VantorSwitch STATIC ${ENGINE_SOURCES})
    target_include_directories(VantorSwitch PUBLIC Source external)

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
    target_link_libraries(VantorSwitch ${SWITCH_LIBRARIES})

    # Setting up Libarie
    install(TARGETS VantorSwitch DESTINATION lib)
    # install(DIRECTORY Source/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # === Create the Executable ===
    add_executable(TestFramework ${APP_SOURCES})
    # Link the engine library to the executable
    target_link_libraries(TestFramework VantorSwitch)
    # Optional: Compiler warnings for the main executable
    target_compile_options(TestFramework PRIVATE -Wall -Wextra)
else()
    message(FATAL_ERROR "Unknown or unsupported platform: ${PLATFORM}")
endif()