cmake_minimum_required(VERSION 3.13)  # CMake version check
project(AVLTree)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set(SOURCE_FILES 
main.cpp 
AVLTree.cpp 
VMVirtAddrManager.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(AVLTree ${SOURCE_FILES})

