cmake_minimum_required(VERSION 3.10)  # CMake version check
cmake_policy(SET  CMP0066 NEW)
project(AVLTree)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard
set(CMAKE_C_FLAGS_DEBUG -DDEBUG)
set(CMAKE_CXX_FLAGS_DEBUG -DDEBUG)
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DDEBUG)


include_directories(./)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

source_group("Headers" FILES ${HEADER_FILES})


if(CMAKE_SYSTEM_NAME STREQUAL Emscripten)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --emrun")

endif()

# set(SOURCE_FILES 
# main.cpp 
# AVLTree.cpp 
# VMVirtAddrManager.cpp)

add_executable(AVLTree ${SOURCE_FILES} ${HEADER_FILES})

# Add executable target with source files listed in SOURCE_FILES variable
# add_executable(AVLTree ${SOURCE_FILES})

