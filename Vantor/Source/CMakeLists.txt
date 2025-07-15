# ==============================================================================
# Vantor Engine Build Configuration
# Author: Lukas Rennhofer @2025
# ==============================================================================

cmake_minimum_required(VERSION 3.10)
project(Vantor VERSION 1.0.0 LANGUAGES CXX C)

# ==============================================================================
# Build Configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-w)

# Build information
message(STATUS "=== Vantor Engine Build Configuration ===")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Toolchain: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Working Directory: $ENV{PWD}")
message(STATUS "========================================")

# ==============================================================================
# Path Constants
# ==============================================================================
set(VANTOR_EXTERNAL_DIR ${CMAKE_CURRENT_LIST_DIR}/../External)
set(VANTOR_SHARED_EXTERNAL ${VANTOR_EXTERNAL_DIR}/Shared)

# ==============================================================================
# Core Engine Sources
# ==============================================================================
set(VANTOR_CORE_SOURCES
    # Common Impl
    VCommonImpl.cpp

    # Application Layer
    Application/VAP_Application.cpp
    
    # Core Systems
    Core/VCO_Version.cpp
    Core/Backlog/VCO_Backlog.cpp
    Core/Debug/VCO_InlineDebugger.cpp
    Core/Global/VCO_ServiceRegistry.cpp

    # Resource Manager
    Resource/VRES_Manager.cpp
    
    # Rendering
    Renderer/Camera/VRE_Camera.cpp
    Renderer/Camera/VRE_Frustum.cpp
    # Renderer (Material)
    Renderer/VRE_Material.cpp
    
    # Input Management
    InputDevice/VID_Manager.cpp
)

# ==============================================================================
# Third-Party Library Sources
# ==============================================================================

# ImGui Core Sources
set(IMGUI_CORE_SOURCES
    ${VANTOR_SHARED_EXTERNAL}/imgui/imgui.cpp
    ${VANTOR_SHARED_EXTERNAL}/imgui/imgui_draw.cpp
    ${VANTOR_SHARED_EXTERNAL}/imgui/imgui_demo.cpp
    ${VANTOR_SHARED_EXTERNAL}/imgui/imgui_widgets.cpp
    ${VANTOR_SHARED_EXTERNAL}/imgui/imgui_tables.cpp
)

# OpenGL Render Device Sources
set(OPENGL_RENDER_SOURCES
    # Add the GLSL Compiler too OpenGL RenderDevice
    RenderDevice/Interface/Compiler/VRD_GLSLComp.cpp

    RenderDevice/OpenGL/VRDO_Shader.cpp
    RenderDevice/OpenGL/VRDO_Mesh.cpp
    RenderDevice/OpenGL/VRDO_Texture.cpp
    RenderDevice/OpenGL/VRDO_RenderTarget.cpp
    RenderDevice/OpenGL/VRDO_UBO.cpp
    RenderDevice/OpenGL/VRDO_CommandBuffer.cpp
    RenderDevice/OpenGL/VRDO_StateCache.cpp
    # Common OpenGL Objects
    RenderDevice/OpenGL/Common/VRDO_ScreenQuad.cpp
    # Main Objects (must be compiled at the end)
    RenderDevice/OpengL/VRDO_RenderPath.cpp
    RenderDevice/OpenGL/VRDO_RenderDevice.cpp
)

# Render Modules (TODO: Make configurable)
set(RENDER_MODULE_SOURCES
    RenderModules/Camera/VRM_FlyCamera.cpp
)

# ==============================================================================
# Platform-Specific Configuration Functions
# ==============================================================================

function(configure_glfw_integration)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW test programs" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW example programs" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documentation" FORCE)
    add_subdirectory(${VANTOR_SHARED_EXTERNAL}/GLFW ${CMAKE_BINARY_DIR}/External/GLFW)
endfunction()

function(setup_imgui_integration backend_sources)
    if(VANTOR_INTEGRATION_IMGUI)
        set(IMGUI_BACKEND_SOURCES ${backend_sources} PARENT_SCOPE)
    endif()
endfunction()

# ==============================================================================
# Platform-Specific Configuration
# ==============================================================================

# Windows Platform Configuration
if(PLATFORM STREQUAL "Windows")
    set(__WINDOWS__ ON)
    message(STATUS "Configuring for Windows platform")

    # Platform-specific source files
    set(PLATFORM_ENVIRONMENT_SOURCES Platform/Platforms/VPL_Windows.cpp)
    
    # Base Windows libraries
    set(PLATFORM_LIBRARIES mingw32 gdi32 user32 imm32 shell32)

    # OpenGL Configuration
    if(VANTOR_API_OPENGL)
        set(RENDER_DEVICE_SOURCES ${OPENGL_RENDER_SOURCES})
        set(PLATFORM_GRAPHICS_SOURCES ${VANTOR_EXTERNAL_DIR}/Windows/windows-glad/glad.c)
        list(APPEND PLATFORM_LIBRARIES opengl32)

        # ImGui OpenGL backend
        setup_imgui_integration("${VANTOR_SHARED_EXTERNAL}/imgui/Backend/imgui_impl_opengl3.cpp")
    endif()

    # GLFW Window Management
    if(VANTOR_WM_GLFW)
        set(CONTEXT_SOURCES Context/Impl/VCT_GLFW3_impl.cpp)
        configure_glfw_integration()
        list(APPEND PLATFORM_LIBRARIES glfw)

        # ImGui GLFW backend
        if(VANTOR_INTEGRATION_IMGUI)
            list(APPEND IMGUI_BACKEND_SOURCES "${VANTOR_SHARED_EXTERNAL}/imgui/Backend/imgui_impl_glfw.cpp")
        endif()
    endif()

# Linux Platform Configuration
elseif(PLATFORM STREQUAL "Linux")
    set(__LINUX__ ON)
    message(STATUS "Configuring for Linux platform")

    # Platform-specific source files
    set(PLATFORM_ENVIRONMENT_SOURCES Platform/Platforms/VPL_Linux.cpp)
    
    # Base Linux libraries
    set(PLATFORM_LIBRARIES X11 pthread dl assimp EGL GLESv2 gbm drm)

    # OpenGL Configuration
    if(VANTOR_API_OPENGL)
        set(RENDER_DEVICE_SOURCES ${OPENGL_RENDER_SOURCES})
        set(PLATFORM_GRAPHICS_SOURCES ${VANTOR_EXTERNAL_DIR}/Linux/linux-glad/glad.c)
        
        # Find OpenGL package
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(OpenGL REQUIRED gl)
        list(APPEND PLATFORM_LIBRARIES ${OpenGL_LIBRARIES})

        # ImGui OpenGL backend
        setup_imgui_integration("${VANTOR_SHARED_EXTERNAL}/imgui/Backend/imgui_impl_opengl3.cpp")
    endif()

    # GLFW Window Management
    if(VANTOR_WM_GLFW)
        set(CONTEXT_SOURCES Context/Impl/VCT_GLFW3_impl.cpp)
        configure_glfw_integration()
        list(APPEND PLATFORM_LIBRARIES glfw)

        # ImGui GLFW backend
        if(VANTOR_INTEGRATION_IMGUI)
            list(APPEND IMGUI_BACKEND_SOURCES "${VANTOR_SHARED_EXTERNAL}/imgui/Backend/imgui_impl_glfw.cpp")
        endif()
    endif()

# Unsupported Platform
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}. Supported platforms: Windows, Linux")
endif()

# ==============================================================================
# Target Creation and Configuration
# ==============================================================================

# Create the main Vantor static library
add_library(Vantor STATIC
    ${VANTOR_CORE_SOURCES}
    ${RENDER_DEVICE_SOURCES}
    ${RENDER_MODULE_SOURCES}
    ${PLATFORM_GRAPHICS_SOURCES}
    ${PLATFORM_ENVIRONMENT_SOURCES}
    ${CONTEXT_SOURCES}
)

# Add ImGui integration if enabled
if(VANTOR_INTEGRATION_IMGUI)
    target_sources(Vantor PRIVATE 
        ${IMGUI_CORE_SOURCES}
        ${IMGUI_BACKEND_SOURCES}
    )
endif()

# ==============================================================================
# Include Directories and Linking
# ==============================================================================

# Set include directories
target_include_directories(Vantor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../External
)

# Apply Vantor-specific definitions
include(../CMake/VantorGlobalDefinitions.cmake)
set_vantor_definitions(Vantor)

# Link libraries
target_link_libraries(Vantor PRIVATE ${PLATFORM_LIBRARIES})

# Compiler options
target_compile_options(Vantor PRIVATE -Wall -Wextra)

# Installation (currently disabled)
# install(TARGETS Vantor DESTINATION lib)

# ==============================================================================
# Build Summary
# ==============================================================================
message(STATUS "=== Vantor Build Summary ===")
message(STATUS "Platform: ${PLATFORM}")
message(STATUS "OpenGL RenderDevice API: ${VANTOR_API_OPENGL}")
message(STATUS "GLFW WM API: ${VANTOR_WM_GLFW}")
message(STATUS "ImGui Integration: ${VANTOR_INTEGRATION_IMGUI}")
message(STATUS "==============================")
