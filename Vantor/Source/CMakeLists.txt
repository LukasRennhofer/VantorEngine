# - CMakeLists Build File written by Lukas Rennhofer @2025 Licensed under MIT  -
# ! This file is currently only for the Vantor´s libraries!

cmake_minimum_required(VERSION 3.10)

# Project name
project(Vantor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilation flags
add_compile_options(-w)

# Include directories
include_directories(../External)

# === Engine Library Sources ===
set(ENGINE_SOURCES
    # External : TODO Move away to follow previous logic
    ../External/Shared/STB/stb_image_impl.cpp
    # Geometry
    Graphics/Geometry/Primitives/vantorCircle.cpp
    Graphics/Geometry/Primitives/vantorCube.cpp
    Graphics/Geometry/Primitives/vantorLine.cpp
    Graphics/Geometry/Primitives/vantorPlane.cpp
    Graphics/Geometry/Primitives/vantorQuad.cpp
    Graphics/Geometry/Primitives/vantorSphere.cpp
    Graphics/Geometry/Primitives/vantorTorus.cpp
    # Renderer
    Graphics/Renderer/Background/vantorBackground.cpp
    Graphics/Renderer/Camera/vantorCamera.cpp
    # platform
    Platform/vantorInput.cpp
    Platform/vantorWindow.cpp
    # TODO: Move away to destination folder
    Platform/Platforms/vantorPlatformWindows.cpp
    Platform/Platforms/vantorPlatformLinux.cpp
    Platform/Platforms/vantorPlatformSwitch.cpp
    # Core
    Core/vantorApplication.cpp
    Core/JobSystem/vantorJobSystem.cpp
    Core/Backlog/vantorBacklog.cpp
    Core/Debug/vantorInlineDebugger.cpp
    Core/Resource/vantorResource.cpp
    Core/Resource/vantorResourceLoader.cpp
    # Entity
    Entity/vantorECS.cpp
    # Utils
    Utils/utils.cpp
    Utils/sdl2Utils.cpp
)

set(RENDERDEVICE_OPENGL
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLShader.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLTexture.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLMaterial.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLChache.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLMesh.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLMaterialLibrary.cpp
    Graphics/RenderDevice/DeviceOpenGL/PBR/vantorOpenGLPBR.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLPostProcessor.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLRenderTarget.cpp
    Graphics/RenderDevice/DeviceOpenGL/vantorOpenGLRenderer.cpp
    # UTILS
    Utils/OpenGL/glError.cpp
)

set(IMGUI_SRC
    GUI/vantorImgui.cpp
    # External : TODO Move away to follow previous logic
    ../External/Shared/imgui/imgui_draw.cpp
    ../External/Shared/imgui/imgui_demo.cpp
    ../External/Shared/imgui/imgui_impl_opengl3.cpp
    ../External/Shared/imgui/imgui_widgets.cpp
    ../External/Shared/imgui/imgui_tables.cpp
    ../External/Shared/imgui/imgui.cpp
)

# === Platform-Specific Dependencies ===
if(PLATFORM STREQUAL "Windows")
    add_definitions(-D__WINDOWS__)
    set(CMAKE_MAKE_PROGRAM "compToolchain/windows/make.exe")
    message(STATUS "Compiling for Windows")

    set(WINDOWS_BINARIES
        ../External/Windows/windows-glad/glad.c
    )

    # Enable API
    add_definitions(-DVANTOR_WM_GLFW) # TODO: MOVE AWAY AND ENABLE MULTI WM LIKE SDL2
    set(VANTOR_WM_GLFW ON)
    # OPENGL
    add_definitions(-DVANTOR_API_OPENGL)
    add_compile_definitions(VANTOR_WM_GLFW)
    set(VANTOR_API_OPENGL ON)
    # === Create the Engine Library for Windows ===
    if (VANTOR_API_OPENGL)
        add_library(VantorWindows STATIC ${ENGINE_SOURCES} ${WINDOWS_BINARIES} ${RENDERDEVICE_OPENGL} ${IMGUI_SRC} External/imgui/imgui_impl_glfw.cpp External/glad.c)
    endif()
    target_include_directories(VantorWindows PUBLIC src external)

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

    # GLFW
    find_package(glfw3 REQUIRED)
    target_link_libraries(VantorWindows glfw)

    target_link_libraries(VantorWindows ${WINDOWS_LIBRARIES} ${SDL2_LIBRARIES})

    # Setting up Libarie
    install(TARGETS VantorWindows DESTINATION lib)
    # install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # Handle Warnings
    target_compile_options(VantorWindows PRIVATE -Wall -Wextra)

elseif(PLATFORM STREQUAL "Linux")
    add_definitions(-D__LINUX__)
    message(STATUS "Compiling for Linux")

    # TODO: Make to Linux Binaries
    set(WINDOWS_BINARIES
        ../External/Windows/windows-glad/glad.c
    )

    # Enable API
    add_definitions(-DVANTOR_WM_GLFW) # TODO: MOVE AWAY AND ENABLE MULTI WM LIKE SDL2
    set(VANTOR_WM_GLFW ON)
    # OPENGL
    add_definitions(-DVANTOR_API_OPENGL)
    add_compile_definitions(VANTOR_WM_GLFW)
    set(VANTOR_API_OPENGL ON)
    # === Create the Engine Library for Windows ===
    if (VANTOR_API_OPENGL)
        add_library(VantorLinux STATIC ${ENGINE_SOURCES} ${WINDOWS_BINARIES} ${RENDERDEVICE_OPENGL} ${IMGUI_SRC} ../External/Shared/imgui/imgui_impl_glfw.cpp)
    endif()
    target_include_directories(VantorLinux PUBLIC src external)

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
    # install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    # Handle Warnings
    target_compile_options(VantorLinux PRIVATE -Wall -Wextra)

    # elseif(PLATFORM STREQUAL "Switch")
    #     add_definitions(-D__SWITCH__)
    #     message(STATUS "Compiling for Nintendo Switch (Homebrew)")

    #     # === Create the Engine Library for Linux ===
    #     if (VANTOR_API_OPENGL)
    #         add_library(VantorSwitch STATIC ${ENGINE_SOURCES} ${RENDERDEVICE_OPENGL})
    #     endif()
    #     target_include_directories(VantorSwitch PUBLIC src external)

    #     set(CMAKE_SYSTEM_NAME Switch)
    #     set(CMAKE_SYSTEM_PROCESSOR aarch64)

    #     set(DEVKITPRO $ENV{DEVKITPRO})
    #     set(DEVKITA64 $ENV{DEVKITA64})
    #     set(CMAKE_C_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-gcc)
    #     set(CMAKE_CXX_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-g++)

    #     include_directories(
    #         ${DEVKITPRO}/libnx/include
    #         ${DEVKITPRO}/portlibs/switch/include
    #     )

    #     link_directories(
    #         ${DEVKITPRO}/libnx/lib
    #         ${DEVKITPRO}/portlibs/switch/lib
    #     )

    #     # Link SDL2 from DevKitPro
    #     include_directories(${DEVKITPRO}/portlibs/switch/include/SDL2)

    #     # Assimp for Switch
    #     find_package(assimp REQUIRED)
    #     set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/assimp")
    #     include_directories(${ASSIMP_INCLUDE_DIR})

    #     add_definitions(-DIMGUI_IMPL_OPENGL_ES2)
    #     add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)


    #     # Include FreeType for Switch
    #     find_package(Freetype REQUIRED)
    #     include_directories(${FREETYPE_INCLUDE_DIRS})

    #     set(SWITCH_LIBRARIES
    #     nx             # Nintendo Switch SDK
    #     GLESv2         # OpenGL ES 2.0 for Switch
    #     drm_nouveau    # For graphics on Switch
    #     mesa           # OpenGL ES implementation
    #     assimp         # Assimp library for Switch
    #     freetype       # Freetype for Switch
    #     -lSDL2          # SDL2 for Switch
    #         ${SDL2_INCLUDE_DIRS}
    #     )
    #     target_link_libraries(VantorSwitch ${SWITCH_LIBRARIES})

    #     # Setting up Libarie
    #     install(TARGETS VantorSwitch DESTINATION lib)
    #     # install(DIRECTORY src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

    #     # Handle Warnings
    #     target_compile_options(VantorSwitch PRIVATE -Wall -Wextra)
else()
    message(FATAL_ERROR "Unknown or unsupported platform: ${PLATFORM}")
endif()
