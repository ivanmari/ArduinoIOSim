cmake_minimum_required(VERSION 3.10)
project(HX711SimDemo)

set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(.)
include_directories(Examples/Blink)
include_directories(HX711)

# Sources
set(SOURCES
    HX711/HX711Sim.cpp
    Examples/HX711Demo.cpp
)

# Executable
add_executable(hx711_demo ${SOURCES})
