cmake_minimum_required(VERSION 2.8.0)

project(AR488-XFR-GUI)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(ZLIB REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Qml CONFIG REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS SerialPort)

add_subdirectory(src)
add_subdirectory(res)
