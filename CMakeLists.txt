cmake_minimum_required(VERSION 3.10)

project(mark)

set(SOURCES 
		main.cpp 
		pipe.cpp
		curl_pipe.cpp
		str_filter.cpp)
set(HEADERS 
		aux.h
		pip.h
		curl_pipe.h
		str_filter.h)

add_executable(mark ${SOURCES})

target_compile_features(mark 
	PUBLIC 
		cxx_std_14)
