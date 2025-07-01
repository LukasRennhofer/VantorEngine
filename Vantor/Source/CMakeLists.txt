# Vantor Build File – Lukas Rennhofer @2025 – MIT License
cmake_minimum_required(VERSION 3.10)
project(Vantor)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-w)

# === Source Groups ===
# ENGINE CORE
set(VANTOR_CORE
    # Application
    Application/VAP_Application.cpp
    # Core
    Core/VCO_Version.cpp
    Core/Backlog/VCO_Backlog.cpp
    Core/Debug/VCO_InlineDebugger.cpp
    # Renderer
    Renderer/Camera/VRE_Camera.cpp
    Renderer/Camera/VRE_Frustum.cpp
    # InputDevice
    InputDevice/VID_Manager.cpp
)

# IMGUI SOURCES
set(IMGUI_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/imgui_tables.cpp
)

# RENDERDEVICE OPENGL
set(RENDERDEVICE_OPENGL
    RenderDevice/OpenGL/VRDO_Shader.cpp
    RenderDevice/OpenGL/VRDO_Mesh.cpp
    RenderDevice/OpenGL/VRDO_RenderDevice.cpp
)

# RENDERMODULES: TODO MOVE AWAY TO CHOOSE MODULES
set(RENDERMODULES
    RenderModules/Camera/VRM_FlyCamera.cpp
)

# === Platform Setup ===

# ++++++ Windows ++++++
if(PLATFORM STREQUAL "Windows")
    set(__WINDOWS__ ON)
    message(STATUS "Target: Windows")

    # External Libraries
    set(LIBS
        mingw32 gdi32 user32 imm32 shell32
        SDL2 SDL2main assimp freetype
    ) # TODO: Move SDL, assimp and freetype

    # Set Platform Environment
    set(PLATFORM_ENV Platform/Platforms/VPL_Windows.cpp)

    # Choose RenderDevice Sources
    if(VANTOR_API_OPENGL)
        set(RENDERDEVICE ${RENDERDEVICE_OPENGL})
        set(PLATFORM_SOURCES ../External/Windows/windows-glad/glad.c)

        set(LIBS ${LIBS} opengl32)
        # : Integrations :
        # IMGUI Integration
        if(VANTOR_INTEGRATION_IMGUI)
            set(IMGUI_SOURCES
                ${IMGUI_SOURCES}
                ../External/Shared/imgui/Backend/imgui_impl_opengl3.cpp
            )
        endif()
    endif()

    # Choose Window Context Sources
    if(VANTOR_WM_GLFW)
        set(CONTEXT_SOURCES Context/Impl/VCT_GLFW3_impl.cpp) # TODO: Abstract for WM Context
        
        set(LIBS ${LIBS} glfw)

        # : Integrations :
        # IMGUI Integration
        if(VANTOR_INTEGRATION_IMGUI)
            set(IMGUI_SOURCES
                ${IMGUI_SOURCES}
                ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/Backend/imgui_impl_glfw.cpp
            )
        endif()
    endif()

# ++++++ Linux ++++++
elseif(PLATFORM STREQUAL "Linux")
    set(__LINUX__ ON)
    message(STATUS "Target: Linux")

    # Set Platform Environment
    set(PLATFORM_ENV Platform/Platforms/VPL_Linux.cpp)
    
    # External Libraries
    set(LIBS
        X11 pthread dl assimp EGL GLESv2 gbm drm
    ) # TODO: Move SDL, assimp and freetype


    # Choose RenderDevice Sources
    if(VANTOR_API_OPENGL)
        set(RENDERDEVICE ${RENDERDEVICE_OPENGL})
        set(PLATFORM_SOURCES ../External/Linux/linux-glad/glad.c)

        set(LIBS ${LIBS} ${OpenGL_LIBRARIES})

        # : Integrations :
        # IMGUI Integration
        if(VANTOR_INTEGRATION_IMGUI)
            set(IMGUI_SOURCES
                ${IMGUI_SOURCES}
                ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/Backend/imgui_impl_opengl3.cpp
            )
        endif()
    endif()

    # Choose Window Context Sources
    if(VANTOR_WM_GLFW)
        set(CONTEXT_SOURCES Context/Impl/VCT_GLFW3_impl.cpp) # TODO: Abstract for WM Context

        set(LIBS ${LIBS} glfw)
        # : Integrations :
        # IMGUI Integration
        if(VANTOR_INTEGRATION_IMGUI)
            set(IMGUI_SOURCES
                ${IMGUI_SOURCES}
                ${CMAKE_CURRENT_LIST_DIR}/../External/Shared/imgui/Backend/imgui_impl_glfw.cpp
            )
        endif()
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenGL REQUIRED gl)
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}")
endif()

# === Final Target ===
add_library(Vantor STATIC
    ${VANTOR_CORE}
    ${RENDERDEVICE}
    ${RENDERMODULES} # TODO: Move away or find solution to choose modules
    ${PLATFORM_SOURCES}
    ${PLATFORM_ENV}
    ${CONTEXT_SOURCES}
)

# === Integrations Target ===
if(VANTOR_INTEGRATION_IMGUI)
    target_sources(Vantor PUBLIC ${IMGUI_SOURCES})
endif()

target_include_directories(Vantor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../External
)

include(../CMake/VantorGlobalDefinitions.cmake)

set_vantor_definitions(Vantor)

# Linking and Installing of Vantor Lib
target_link_libraries(Vantor ${LIBS})
target_compile_options(Vantor PRIVATE -Wall -Wextra)
# install(TARGETS Vantor DESTINATION lib)
