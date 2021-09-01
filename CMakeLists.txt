cmake_minimum_required(VERSION 3.0)  # CMake version check
project(AVLTree)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard
set(CMAKE_C_FLAGS_DEBUG -DDEBUG)
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DDEBUG)


include_directories(./)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

source_group("Headers" FILES ${HEADER_FILES})


# set(SOURCE_FILES 
# main.cpp 
# AVLTree.cpp 
# VMVirtAddrManager.cpp)

add_executable(AVLTree ${SOURCE_FILES} ${HEADER_FILES})

# Add executable target with source files listed in SOURCE_FILES variable
# add_executable(AVLTree ${SOURCE_FILES})

