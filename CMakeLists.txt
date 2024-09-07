# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(MyProject C)

# Enable all warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Include header files from the current directory
include_directories(.)

# Get all .c files in the current directory
file(GLOB SOURCES "*.c")

# Create the executable from all .c files
add_executable(my_program ${SOURCES})
