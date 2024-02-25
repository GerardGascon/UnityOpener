cmake_minimum_required(VERSION 3.28)
project(UnityOpener C)

set(CMAKE_C_STANDARD 11)

add_executable(UnityOpener main.c
        unity.h
        unity.c)
