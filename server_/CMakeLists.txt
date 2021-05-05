cmake_minimum_required(VERSION 3.5)

project(server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)


  add_executable(server
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mytcpserver.h
    mytcpserver.cpp
    mainwindow.ui
  )


target_link_libraries(server PRIVATE Qt5::Widgets Qt5::Network)
