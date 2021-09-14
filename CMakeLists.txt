cmake_minimum_required(VERSION 3.20)  # CMake version check

 # Enable IPO 
cmake_policy(SET CMP0069 NEW)
cmake_policy(SET  CMP0066 NEW)

# execute_process(COMMAND cmake -P "../GitVersion.cmake" "version.cpp")

add_custom_command(OUTPUT "version.cpp" COMMAND cmake -P "${CMAKE_SOURCE_DIR}/GitVersion.cmake" "${CMAKE_BINARY_DIR}/version.cpp" DEPENDS GitVersion.cmake)

project(AVLTree)               # Create project "simple_example"
include(CTest)
enable_testing()
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
JavascriptTree.cpp
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
	add_compile_options(--bind)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --bind --emrun")

endif()

add_library(AVLTreeLib OBJECT ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})


if( Catch2_FOUND )
	message("Catch2 Found")
	set(UNIT_TEST AVLTests)
	add_executable(${UNIT_TEST} AVLTests.cpp ${HEADER_FILES} )
	target_link_libraries(${UNIT_TEST} PRIVATE AVLTreeLib Catch2::Catch2WithMain)
	add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})
add_custom_command(
     TARGET ${UNIT_TEST}
     COMMENT "Run tests"
     POST_BUILD 
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> -R "^${UNIT_TEST}$" --output-on-failures)

endif()

add_executable(AVLTree main.cpp ${HEADER_FILES})
target_link_libraries(AVLTree PRIVATE AVLTreeLib)

# Add executable target with source files listed in SOURCE_FILES variable
# add_executable(AVLTree ${SOURCE_FILES})

