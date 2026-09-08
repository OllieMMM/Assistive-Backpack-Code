cmake_minimum_required(VERSION 3.20)

project(BackpackCore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(backpack
    firmware/backpack-core/src/main.cpp
    firmware/backpack-core/src/Backpack.cpp
    firmware/backpack-core/src/Inventory.cpp
    firmware/backpack-core/src/TaskManager.cpp
)

target_include_directories(backpack PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)