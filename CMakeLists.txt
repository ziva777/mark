cmake_minimum_required(VERSION 3.10)

project(mark)

set(SOURCES 
		main.cpp 
		pipe.c
		curl_pipe.cpp)
set(HEADERS 
		pip.h
		curl_pipe.h)

add_executable(mark ${SOURCES})

target_compile_features(mark 
	PUBLIC 
		cxx_std_14)
