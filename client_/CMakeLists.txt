cmake_minimum_required(VERSION 3.5)

project(client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)


  add_executable(client
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mytcpclient.h
    mytcpclient.cpp
    mainwindow.ui
  )


target_link_libraries(client PRIVATE Qt5::Widgets Qt5::Network)
