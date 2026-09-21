// TaskManager.h
// This header file defines the TaskManager class, which manages a collection 
// of tasks in a backpack task management system.
// We do not define the implementation of the functions here, only their declarations. 
// The actual implementation will be in the corresponding TaskManager.cpp file.

#pragma once

#include <cstdint>

#include "Config.h"
#include "Inventory.h"
#include "Task.h"

class TaskManager
{
public:

    explicit TaskManager(Inventory& inventory);

    bool registerTask(const Task& task);

    bool startTask(TaskType type);

    void stopTask();

    TaskType getCurrentTask() const;

    ItemType getNextRequiredItem() const;

    bool isTaskComplete() const;

    bool isTaskActive() const;

    bool isItemRequired(ItemType type) const;

private:

    Inventory& inventory;

    Task tasks[MAX_TASKS];
    uint8_t taskCount = 0;

    TaskType activeTask = TaskType::None;
    bool taskActive = false;

    const Task* findTask(TaskType type) const;
    
};