// Config.h
// This header file contains configuration settings for the Backpack Core project.

#pragma once

#include <cstdint> // Allows us to use the uint8_t data type, which is an unsigned 8-bit integer.

constexpr uint8_t MAX_TASK_ITEMS = 10; // Defines the maximum number of task items that can be stored.
constexpr uint8_t MAX_ITEMS = 20; // Defines the maximum number of items that can be stored in backpack inventory manager.
constexpr uint8_t MAX_TASKS = 5; // Defines the maximum number of tasks that can be stored.
constexpr uint8_t MAX_EVENTS = 10; // Define tha maximum number of Events that can be held in the event queue