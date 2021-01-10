cmake_minimum_required(VERSION 3.0)

project(DConsole)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

if(UNIX)
    set(PLATFORM_HEADERS
        Source/unix/Socket.h
    )
    set(PLATFORM_SOURCES
        Source/unix/Socket.cpp
    )
endif(UNIX)

if(WIN32)
set(PLATFORM_HEADERS
        Source/win/WPipe.h
    )
    set(PLATFORM_SOURCES
        Source/win/WPipe.cpp
    )
endif(WIN32)


set(SOURCES 
    Source/main.cpp 
    Source/MainWindow.cpp
    Source/MainTabWidget.cpp
    Source/IPC.cpp
    Source/utils.cpp
    ${PLATFORM_SOURCES}
)

set(HEADERS 
    Source/MainWindow.h
    Source/MainTabWidget.h 
    Source/IPC.h
    Source/VIPC.h
    Source/utils.h
    ${PLATFORM_HEADERS}
)

# This will create you executable
add_executable(DConsole ${SOURCES} ${HEADERS})
# This will link necessary Qt5 libraries to your project
target_link_libraries(DConsole Qt5::Widgets Qt5::Network )
