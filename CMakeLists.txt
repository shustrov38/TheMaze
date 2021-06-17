cmake_minimum_required(VERSION 3.17)
project(TheMaze C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_DIRECTORY src)

add_subdirectory(${SOURCE_DIRECTORY}/server)
#add_subdirectory(${SOURCE_DIRECTORY}/client)
add_subdirectory(${SOURCE_DIRECTORY}/local-game)

