cmake_minimum_required(VERSION 2.8)
project(CHIFEngine)

# COMMON SETTINGS FOR SPE*
include(src/common/icg_settings.cmake)

# engine
add_subdirectory(src/core)
