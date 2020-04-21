cmake_minimum_required(VERSION 3.5.1)

project(gameboy_exe)

add_executable(gameboy src/gameboy.c)

add_library(
        entry_point_validator
        include/entry_point_validator.h
        src/entry_point_validator.c)

target_link_libraries(entry_point_validator -lcommons)
target_link_libraries(gameboy gameboy_utils)