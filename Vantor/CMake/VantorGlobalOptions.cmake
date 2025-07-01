# CMake/VantorGlobalOptions.cmake
# This file is used for getting all possible compile options from the user-end

# The Platform
if(PLATFORM STREQUAL "Windows") # Platform can be get through the PLATFORM VAR
    set(__WINDOWS__ ON)
elseif(PLATFORM STREQUAL "Linux")
    set(__LINUX__ ON)
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}")
endif()

# ==== Options ====
# Compile Options
option(VANTOR_API_OPENGL "Use OpenGL backend" ON)
option(VANTOR_WM_GLFW "Use GLFW window manager" ON)
# Integrations
option(VANTOR_INTEGRATION_IMGUI "Use ImGui as Integration" ON)

# Export the settings as cache variables so they're accessible project-wide
# Use FORCE to ensure values are set even if cache is corrupted
set(__WINDOWS__ ${__WINDOWS__} CACHE BOOL "Build on platform Windows" FORCE)
set(__LINUX__ ${__LINUX__} CACHE BOOL "Build on platform Linux" FORCE)
set(VANTOR_API_OPENGL ${VANTOR_API_OPENGL} CACHE BOOL "Use OpenGL backend" FORCE)
set(VANTOR_WM_GLFW ${VANTOR_WM_GLFW} CACHE BOOL "Use GLFW window manager" FORCE)
set(VANTOR_INTEGRATION_IMGUI ${VANTOR_INTEGRATION_IMGUI} CACHE BOOL "Enable ImGui integration" FORCE)

# Also ensure variables are available in current scope and parent scope
set(__WINDOWS__ ${__WINDOWS__})
set(__LINUX__ ${__LINUX__})
set(VANTOR_API_OPENGL ${VANTOR_API_OPENGL})
set(VANTOR_WM_GLFW ${VANTOR_WM_GLFW})
set(VANTOR_INTEGRATION_IMGUI ${VANTOR_INTEGRATION_IMGUI})