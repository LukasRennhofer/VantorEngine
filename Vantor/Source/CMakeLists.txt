# Vantor Build File – Lukas Rennhofer @2025 – MIT License
cmake_minimum_required(VERSION 3.10)
project(Vantor)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-w)

# Compile Options
option(VANTOR_API_OPENGL "Use OpenGL backend" ON)
option(VANTOR_WM_GLFW "Use GLFW window manager" ON)

# === Source Groups ===
# ENGINE CORE
set(VANTOR_CORE
    # Application
    Application/VAP_Application.cpp
    # Core
    Core/VCO_Version.cpp
    Core/Backlog/VCO_Backlog.cpp
    Core/Debug/VCO_InlineDebugger.cpp
    # Entity
    Entity/vantorECS.cpp
)
# IMGUI SOURCES
set(IMGUI_SOURCES
    ../External/Shared/imgui/imgui.cpp
    ../External/Shared/imgui/imgui_draw.cpp
    ../External/Shared/imgui/imgui_demo.cpp
    ../External/Shared/imgui/imgui_widgets.cpp
    ../External/Shared/imgui/imgui_tables.cpp
    ../External/Shared/imgui/imgui_impl_opengl3.cpp
    ../External/Shared/imgui/imgui_impl_glfw.cpp
)

# RENDERDEVICE OPENGL
set(RENDERDEVICE_OPENGL
    RenderDevice/OpenGL/VRDO_Shader.cpp
    RenderDevice/OpenGL/VRDO_RenderDevice.cpp
)

# === Platform Setup ===

# ++++++ Windows ++++++
if(PLATFORM STREQUAL "Windows")
    add_definitions(-D__WINDOWS__) # Platform Preprocessor
    message(STATUS "Target: Windows")

    # Set Platform Environment
    set(PLATFORM_ENV Platform/Platforms/VPL_Windows.cpp)

    # Choose RenderDevice Sources
    if(VANTOR_API_OPENGL)
        add_definitions(-DVANTOR_API_OPENGL)
        set(RENDERDEVICE ${RENDERDEVICE_OPENGL})
        set(PLATFORM_SOURCES ../External/Windows/windows-glad/glad.c)
    endif()

    # Choose Window Context Sources
    if(VANTOR_WM_GLFW)
        add_definitions(-DVANTOR_WM_GLFW)
        set(CONTEXT_SOURCES Context/Impl/VCT_GLFW3_impl.cpp) # TODO: Abstract for WM Context
    endif()

    set(LIBS
        mingw32 opengl32 gdi32 user32 imm32 shell32
        SDL2 SDL2main glfw assimp freetype
    )

# ++++++ Linux ++++++
elseif(PLATFORM STREQUAL "Linux")
    add_definitions(-D__LINUX__) # Platform Preprocessor
    message(STATUS "Target: Linux")

    # Set Platform Environment
    set(PLATFORM_ENV Platform/Platforms/VPL_Linux.cpp)

    # Choose RenderDevice Sources
    if(VANTOR_API_OPENGL)
        add_definitions(-DVANTOR_API_OPENGL)
        set(RENDERDEVICE ${RENDERDEVICE_OPENGL})
        set(PLATFORM_SOURCES ../External/Linux/linux-glad/glad.c)
    endif()

    # Choose Window Context Sources
    if(VANTOR_WM_GLFW)
        add_definitions(-DVANTOR_WM_GLFW)
        set(CONTEXT_SOURCES Context/Impl/VCT_GLFW3_impl.cpp) # TODO: Abstract for WM Context
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenGL REQUIRED gl)

    set(LIBS
        ${OpenGL_LIBRARIES} glfw
        X11 pthread dl assimp EGL GLESv2 gbm drm
    )

else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}")
endif()

# === Final Target ===
add_library(Vantor STATIC
    ${VANTOR_CORE}
    ${RENDERDEVICE}
    ${IMGUI_SOURCES}
    ${PLATFORM_SOURCES}
    ${PLATFORM_ENV}
    ${CONTEXT_SOURCES}
)

target_include_directories(Vantor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../External
)

# Main Preprocessors:
if(VANTOR_API_OPENGL)
    target_compile_definitions(Vantor PUBLIC VANTOR_API_OPENGL)
endif()

if(VANTOR_WM_GLFW)
    target_compile_definitions(Vantor PUBLIC VANTOR_WM_GLFW)
endif()

if(PLATFORM STREQUAL "Windows")
    target_compile_definitions(Vantor PUBLIC __WINDOWS__)
elseif(PLATFORM STREQUAL "Linux")
    target_compile_definitions(Vantor PUBLIC __LINUX__)
endif()

# Linking and Installing of Vantor Lib
target_link_libraries(Vantor  ${LIBS})
target_compile_options(Vantor PRIVATE -Wall -Wextra)
install(TARGETS Vantor DESTINATION lib)
