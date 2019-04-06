cmake_minimum_required(VERSION 3.10)

project(mark)
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp)

add_executable(mark ${SOURCES})

target_compile_features(mark 
	PUBLIC 
		cxx_std_14)
