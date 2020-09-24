cmake_minimum_required(VERSION 3.0)

project(DConsole)


add_executable(DConsole 
Source/main.cpp
)


set(CoreUI_DIR Source/CoreUI)
set(Core_DIR Source/Core)

add_subdirectory(${CoreUI_DIR})
add_subdirectory(${Core_DIR})

target_link_libraries(DConsole Core CoreUI)