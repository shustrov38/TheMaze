cmake_minimum_required(VERSION 3.17)
project(TheMaze C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_DIRECTORY src)

set(SOURCE_FILES
        ${SOURCE_DIRECTORY}/utilities.c
        ${SOURCE_DIRECTORY}/utilities.h
        ${SOURCE_DIRECTORY}/ok_lib.h
		${SOURCE_DIRECTORY}/sqlite3.h
)

add_subdirectory(${SOURCE_DIRECTORY}/server)
add_subdirectory(${SOURCE_DIRECTORY}/client)