cmake_minimum_required(VERSION 3.3)
project(assignment3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    mycat.c
    mycmp.c
    mycp.c
    myenv.c
    myhead.c
    myls.c
    mymkdir.c
    mymv.c
    myps.c
    mypwd.c
    myrm.c
    myrmdir.c
    myshell.c
    mysort.c
    mytail.c)

add_executable(assignment3 ${SOURCE_FILES})