cmake_minimum_required(VERSION 3.0)

project(Core)

add_library(Core target.cpp)

if(WIN32)
    include(PlatformWin.cmake)
    target_link_libraries(Core WinCore)
endif(WIN32)

if(UNIX)
    include(PlatformLinux.cmake)
    target_link_libraries(Core LinuxCore ${GTK2_LIBRARIES})
endif(UNIX)

