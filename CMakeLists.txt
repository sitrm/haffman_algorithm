cmake_minimum_required(VERSION 3.28)
#mkdir build 
#cmake . -B build 
#cmake [options] -S <path-to-source> -B <path-to-build>
#cmake --build ./build --config debug
set(PROJECT_NAME haffman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

#set(CMAKE_C_COMPILER "gcc-13.2.0")
#set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++")

project(${PROJECT_NAME} LANGUAGES CXX)

#add_executable(projName [win32] [macos] [EXCLUDE_FROM_ALL] source1 [source2])

add_executable(${PROJECT_NAME}   ./src_haffman/main.cpp ./src_haffman/haffman.cpp)
#add_executable(${PROJECT_NAME}   ./src_haffman/main_test.cpp)
#ALL ИЛИ ALL_BUILD

