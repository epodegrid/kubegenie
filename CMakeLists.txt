cmake_minimum_required(VERSION 3.16)
project(k8sgenie C)

set(CMAKE_C_STANDARD 99)

add_executable(k8sgenie src/k8sgenie.c
        src/error.c src/error.h
        src/init.c src/init.h
        src/datastruct.c src/datastruct.h
        src/strop.c src/strop.h
        src/runner.c src/runner.h
        src/io.c src/io.h
        src/ckv.c src/ckv.h
        src/genie.c src/genie.h
        src/switchs.h)
target_link_libraries(k8sgenie jansson)
