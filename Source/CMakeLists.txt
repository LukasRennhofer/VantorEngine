# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the CHIFEngine´s libraries!

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

# TODO: This is meant for building the Engine with OpenGL everytime, so if building for another API move this away
add_definitions(-DCHIF_API_OPENGL)
set(CHIF_API_OPENGL ON)

# === Engine Library Sources ===
set(ENGINE_SOURCES
    # External
    External/imgui/imgui_draw.cpp
    External/imgui/imgui_demo.cpp
    External/imgui/imgui_impl_sdl2.cpp
    External/imgui/imgui_impl_opengl3.cpp
    External/imgui/imgui_widgets.cpp
    External/imgui/imgui_tables.cpp
    External/imgui/imgui.cpp
    External/stb_image_impl.cpp
    External/glad.c
    # Graphics
    Graphics/chifDrawableObject.cpp
    Graphics/chifCloudsModel.cpp
    Graphics/chifSkybox.cpp
    Graphics/chifVolumetricClouds.cpp
    Graphics/chifWater.cpp
    Graphics/chifTerrain.cpp
    Graphics/chifTexture.cpp
    # Graphics / Camera
    Graphics/Camera/chifCamera.cpp
    # Renderer
    Graphics/Renderer/chifBuffers.cpp
    Graphics/Renderer/chifBaseShader.cpp
    Graphics/Renderer/chifShader.cpp
    Graphics/Renderer/chifScreenSpaceShader.cpp
    # platform
    Platform/chifInput.cpp
    Platform/chifWindow.cpp
    # TODO: Move away to destination folder
    Platform/Platforms/chifPlatformWindows.cpp
    Platform/Platforms/chifPlatformLinux.cpp
    Platform/Platforms/chifPlatformSwitch.cpp
    # GUI
    GUI/chifImgui.cpp
    # Core
    Core/chifApplication.cpp
    Core/ResourceManager/chifResourceManager.cpp
    Core/JobSystem/chifJobSystem.cpp 
    Core/Backlog/chifBacklog.cpp
    Core/Debug/chifInlineDebugger.cpp
    # Entity
    Entity/chifECS.cpp
    # Utils
    Utils/utils.cpp
    Utils/sdl2Utils.cpp
    Utils/OpenGL/glError.cpp
)

set(RENDERDEVICE_OPENGL
    Graphics/RenderDevice/DeviceOpenGL/chifOpenGLShader.cpp
    Graphics/RenderDevice/DeviceOpenGL/chifOpenGLTexture.cpp
    Graphics/RenderDevice/DeviceOpenGL/chifOpenGLMaterial.cpp
    Graphics/RenderDevice/DeviceOpenGL/chifOpenGLChache.cpp
    Graphics/RenderDevice/DeviceOpenGL/chifOpenGLMesh.cpp
)

# === Platform-Specific Dependencies ===
if(PLATFORM STREQUAL "Windows")
    add_definitions(-D__WINDOWS__)  
    set(CMAKE_MAKE_PROGRAM "compToolchain/windows/make.exe")
    message(STATUS "Compiling for Windows")

    # === Create the Engine Library for Windows ===
    if (CHIF_API_OPENGL)
        add_library(CHIFEngineWindows STATIC ${ENGINE_SOURCES} ${RENDERDEVICE_OPENGL})
    endif()
    target_include_directories(CHIFEngineWindows PUBLIC src external)

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
    # install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # Handle Warnings
    target_compile_options(CHIFEngineWindows PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Linux")
    add_definitions(-D__LINUX__)
    message(STATUS "Compiling for Linux")

    # === Create the Engine Library for Linux ===
    if (CHIF_API_OPENGL)
        add_library(CHIFEngineLinux STATIC ${ENGINE_SOURCES} ${RENDERDEVICE_OPENGL})
    endif()
    target_include_directories(CHIFEngineLinux PUBLIC src external)

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
    # install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # Handle Warnings
    target_compile_options(CHIFEngineLinux PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Switch")
    add_definitions(-D__SWITCH__)
    message(STATUS "Compiling for Nintendo Switch (Homebrew)")

    # === Create the Engine Library for Linux ===
    if (CHIF_API_OPENGL)
        add_library(CHIFEngineSwitch STATIC ${ENGINE_SOURCES} ${RENDERDEVICE_OPENGL})
    endif()
    target_include_directories(CHIFEngineSwitch PUBLIC src external)

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
    # install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # Handle Warnings
    target_compile_options(CHIFEngineSwitch PRIVATE -Wall -Wextra)
else()
    message(FATAL_ERROR "Unknown or unsupported platform: ${PLATFORM}")
endif()