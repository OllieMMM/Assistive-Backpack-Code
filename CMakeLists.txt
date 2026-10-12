cmake_minimum_required(VERSION 3.20)

project(BackpackCore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(backpack
    firmware/backpack-core/src/main.cpp
    firmware/backpack-core/src/DebugConsole.cpp

    # Shared core implementation — used by Arduino too.
    firmware/libraries/AssistiveBackpackCore/src/Backpack.cpp
    firmware/libraries/AssistiveBackpackCore/src/Inventory.cpp
    firmware/libraries/AssistiveBackpackCore/src/TaskManager.cpp
    firmware/libraries/AssistiveBackpackCore/src/EventGenerator.cpp
)

target_include_directories(backpack
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/backpack-core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/firmware/libraries/AssistiveBackpackCore/src
)
