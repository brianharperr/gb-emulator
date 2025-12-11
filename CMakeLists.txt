cmake_minimum_required(VERSION 3.24)
project(GameBoyEmu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build SDL3 from source included in the project
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# Automatically gather all source files
file(GLOB_RECURSE GAMEBOY_SOURCES src/*.cpp)

add_executable(gameboy_emu ${GAMEBOY_SOURCES})

target_include_directories(gameboy_emu PRIVATE include)

target_link_libraries(gameboy_emu PRIVATE SDL3::SDL3)
