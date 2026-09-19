// Task.h
// This header file defines the structure and types for tasks in the Backpack
// Here the preset tasks are defined. For V1 this is unchanging.

#pragma once

#include <cstdint>

#include "Config.h"
#include "Item.h"

enum class TaskType
{
    University,
    Gym,
    Work
};

struct Task
{
    TaskType type;
    ItemType requiredItems[MAX_TASK_ITEMS]; // Max item cpacity
    uint8_t itemCount;  // Number of items in the task
    uint8_t reminderAudio;
};