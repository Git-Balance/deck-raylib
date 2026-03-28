cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
project(deck)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
        ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)  

add_executable(deck
    src/main.cpp
)

target_link_libraries(deck PRIVATE raylib)
target_link_libraries(deck PRIVATE glfw)
