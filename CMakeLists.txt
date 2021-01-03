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

set(SOURCES 
    Source/main.cpp 
    Source/MainWindow.cpp 
    Source/Sockets.cpp
    Source/utils.cpp
) 
set(HEADERS 
    Source/MainWindow.h 
    Source/Sockets.h
    Source/utils.h
)

# This will create you executable
add_executable(DConsole ${SOURCES} ${HEADERS})
# This will link necessary Qt5 libraries to your project
target_link_libraries(DConsole Qt5::Widgets Qt5::Network )
