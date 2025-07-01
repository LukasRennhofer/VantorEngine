# CMake/VantorGlobalDefinitions.cmake
# This file defines all global preprocessors (for compile time)

include(${CMAKE_CURRENT_LIST_DIR}/VantorGlobalOptions.cmake)

function(set_vantor_definitions target)
    # Get values from cache first, then fallback to direct variables
    get_property(VANTOR_API_OPENGL_VAL CACHE VANTOR_API_OPENGL PROPERTY VALUE)
    get_property(VANTOR_WM_GLFW_VAL CACHE VANTOR_WM_GLFW PROPERTY VALUE)
    get_property(VANTOR_INTEGRATION_IMGUI_VAL CACHE VANTOR_INTEGRATION_IMGUI PROPERTY VALUE)
    get_property(__WINDOWS___VAL CACHE __WINDOWS__ PROPERTY VALUE)
    get_property(__LINUX___VAL CACHE __LINUX__ PROPERTY VALUE)
    
    # Fallback to direct variables if cache is empty
    if(NOT DEFINED VANTOR_API_OPENGL_VAL OR VANTOR_API_OPENGL_VAL STREQUAL "")
        set(VANTOR_API_OPENGL_VAL ${VANTOR_API_OPENGL})
    endif()
    if(NOT DEFINED VANTOR_WM_GLFW_VAL OR VANTOR_WM_GLFW_VAL STREQUAL "")
        set(VANTOR_WM_GLFW_VAL ${VANTOR_WM_GLFW})
    endif()
    if(NOT DEFINED VANTOR_INTEGRATION_IMGUI_VAL OR VANTOR_INTEGRATION_IMGUI_VAL STREQUAL "")
        set(VANTOR_INTEGRATION_IMGUI_VAL ${VANTOR_INTEGRATION_IMGUI})
    endif()
    if(NOT DEFINED __WINDOWS___VAL OR __WINDOWS___VAL STREQUAL "")
        set(__WINDOWS___VAL ${__WINDOWS__})
    endif()
    if(NOT DEFINED __LINUX___VAL OR __LINUX___VAL STREQUAL "")
        set(__LINUX___VAL ${__LINUX__})
    endif()

    if(VANTOR_API_OPENGL_VAL)
        target_compile_definitions(${target} PRIVATE VANTOR_API_OPENGL)
    endif()

    if(VANTOR_WM_GLFW_VAL)
        target_compile_definitions(${target} PRIVATE VANTOR_WM_GLFW)
    endif()

    if(VANTOR_INTEGRATION_IMGUI_VAL)
        target_compile_definitions(${target} PRIVATE VANTOR_INTEGRATION_IMGUI)
    endif()

    if(__WINDOWS___VAL)
        target_compile_definitions(${target} PRIVATE __WINDOWS__)
    elseif(__LINUX___VAL)
        target_compile_definitions(${target} PRIVATE __LINUX__)
    endif()
endfunction()