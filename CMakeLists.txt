cmake_minimum_required(VERSION 3.10)  # CMake version check

 # Enable IPO 
cmake_policy(SET CMP0069 NEW)
cmake_policy(SET  CMP0066 NEW)

execute_process(COMMAND cmake -P "../GitVersion.cmake" "version.cpp")

project(AVLTree)               # Create project "simple_example"
include(CTest)
find_package(Catch2 3)


set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard
# set(CMAKE_C_FLAGS_DEBUG -DDEBUG)
# set(CMAKE_CXX_FLAGS_DEBUG -DDEBUG)
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DDEBUG)


include_directories(./)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

set(LIBRARY_SOURCES AVLTree.cpp
html_template.cpp
TreeDumpers.cpp
VMVirtAddrManager.cpp
version.cpp)

set(LIBRARY_HEADERS AVLTree.h
AVLTreeT.h
BinaryTreeNodeT.h
Colors.h
PrintTree.h
TreeDumpers.h
VMVirtAddrManager.h)


source_group("Headers" FILES ${LIBRARY_HEADERS})


if(CMAKE_SYSTEM_NAME STREQUAL Emscripten)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --emrun")

endif()

add_library(AVLTreeLib OBJECT ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
add_executable(AVLTree main.cpp ${HEADER_FILES})
target_link_libraries(AVLTree PRIVATE AVLTreeLib)

if( Catch2_FOUND )
	message("Catch2 Found")
	add_executable(AVLTests AVLTests.cpp ${HEADER_FILES} )
	target_link_libraries(AVLTests PRIVATE AVLTreeLib Catch2::Catch2WithMain)
endif()


# Add executable target with source files listed in SOURCE_FILES variable
# add_executable(AVLTree ${SOURCE_FILES})

