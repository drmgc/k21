cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(k21-ppl CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(k21-ppl ${SRC})
target_include_directories(k21-ppl
	PUBLIC
	3rd/cxxopts
)
