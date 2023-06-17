cmake_minimum_required(VERSION 3.2 )

set(PROJECT_NAME csvreader)
project(${PROJECT_NAME} VERSION 1.0)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/tableview.cpp
    src/tablemodel.cpp
)
