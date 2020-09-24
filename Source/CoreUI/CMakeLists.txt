cmake_minimum_required(VERSION 3.0)

project(CoreUI)

add_library(CoreUI target.cpp)

if(WIN32)
    include(PlatformWin.cmake)
    target_link_libraries(CoreUI WinUI)
endif(WIN32)

if(UNIX)
    include(PlatformLinux.cmake)
    target_link_libraries(CoreUI LinuxUI)
endif(UNIX)

