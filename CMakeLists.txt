cmake_minimum_required(VERSION 3.0)  # CMake version check
project(AVLTree)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard

set(CMAKE_C_FLAGS_DEBUG -DDEBUG)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(SOURCE_FILES 
main.cpp 
AVLTree.cpp 
VMVirtAddrManager.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(AVLTree ${SOURCE_FILES})

