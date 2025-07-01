# CMake/VantorCMake.cmake
# This file can be used to easily include Vantor into your cmake project

include(${CMAKE_CURRENT_LIST_DIR}/VantorGlobalOptions.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/VantorGlobalDefinitions.cmake)

function(SetUpVantorTarget target)
    message(STATUS "Setting up Vantor definitions for target: ${target}")
    set_vantor_definitions(${target})
endfunction()