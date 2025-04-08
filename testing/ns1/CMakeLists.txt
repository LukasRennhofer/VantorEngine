cmake_minimum_required(VERSION 3.15)
project(SwitchApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# DevkitPro toolchain
if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "Please set DEVKITPRO in your environment.")
endif()

set(DEVKITPRO $ENV{DEVKITPRO})
set(LIBNX ${DEVKITPRO}/libnx)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER /opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc)
set(CMAKE_CXX_COMPILER /opt/devkitpro/devkitA64/bin/aarch64-none-elf-g++)
set(CMAKE_ASM_COMPILER ${DEVKITA64}/bin/aarch64-none-elf-as)
set(CMAKE_AR ${DEVKITA64}/bin/aarch64-none-elf-gcc-ar)
set(CMAKE_RANLIB ${DEVKITA64}/bin/aarch64-none-elf-gcc-ranlib)


set(SWITCH_SPECS ${LIBNX}/switch.specs)
set(CMAKE_EXE_LINKER_FLAGS "-specs=${SWITCH_SPECS} -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE -pie -Wl,-Map=output.map")

include_directories($ENV{DEVKITPRO}/portlibs/switch/include)
link_directories($ENV{DEVKITPRO}/portlibs/switch/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${LIBNX}/include
    ${PORTLIBS}/include
)

link_directories(
    ${LIBNX}/lib
    ${PORTLIBS}/lib
)

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/source/*.c
    ${PROJECT_SOURCE_DIR}/source/*.cpp
    ${PROJECT_SOURCE_DIR}/source/*.s
)

file(GLOB DATA_FILES
    ${PROJECT_SOURCE_DIR}/data/*
)

add_executable(${PROJECT_NAME} ${SOURCES} ${DATA_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE __SWITCH__)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -O2 -ffunction-sections -fPIE
    -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti -fno-exceptions>
)

target_link_libraries(${PROJECT_NAME}
    -lnx
    -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
)

# RomFS support
set(ROMFS_DIR ${PROJECT_SOURCE_DIR}/romfs)
if(EXISTS ${ROMFS_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${DEVKITPRO}/tools/bin/nxlink -r ${ROMFS_DIR} ${PROJECT_NAME}.elf)
endif()

# NRO creation
find_program(ELF2NRO elf2nro PATHS ${DEVKITPRO}/tools/bin)
if(NOT ELF2NRO)
    message(FATAL_ERROR "elf2nro not found!")
endif()

set(NACP_TITLE "SDL2+mixer+image Demo")
set(NACP_AUTHOR "carstene1ns")
set(NACP_VERSION "1.0.0")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${ELF2NRO} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro
        --icon=${PROJECT_SOURCE_DIR}/icon.jpg
        --nacp=${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.nacp
        --romfsdir=${ROMFS_DIR}
)
