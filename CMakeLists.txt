cmake_minimum_required(VERSION 3.3)
project(myShell)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp myls.c.c myls.c.h myls.c test.c mycp.c myrm.c myrmdir.c directory2/mymv.c mymkdir.c)
add_executable(myShell ${SOURCE_FILES})