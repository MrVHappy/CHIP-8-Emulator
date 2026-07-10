cmake_minimum_required(VERSION 3.10)
project(CHIP8Emulator)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Add executable
add_executable(chip8 src/main.cpp)

# Link SDL2
target_link_libraries(chip8 ${SDL2_LIBRARIES})