// TaskManager.cpp
// This source file implements the functionality of the Task Manager.

#include "TaskManager.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Initialize this TaskManager's inventory reference using the Inventory supplied to the constructor.
TaskManager::TaskManager(Inventory& inventory)
    : inventory(inventory)
{
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Registers a new task in the TaskManager with the given task.
// Returns true if the task was successfully registered, false otherwise.
bool TaskManager::registerTask(const Task& task)
{
    if (taskCount >= MAX_TASKS)
    {
        return false;
    }

    for (uint8_t i = 0; i < taskCount; i++)
    {
        if (tasks[i].type == task.type)
        {
            return false;
        }
    }

    tasks[taskCount] = task;
    taskCount++;

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Starts a task of the specified type if it is registered and not already active.
// Returns true if the task was successfully started, false otherwise.
bool TaskManager::startTask(TaskType type)
{
    for (uint8_t i = 0; i < taskCount; i++)
    {
        if (tasks[i].type == type)
        {
            activeTask = type;
            taskActive = true;

            return true;
        }
    }

    return false;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Stops the currently active task, if any.
void TaskManager::stopTask()
{
    activeTask = TaskType::None;
    taskActive = false;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Gets the current active task type.
// Returns TaskType::None if no task is active.
TaskType TaskManager::getCurrentTask() const
{
    return activeTask;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Finds a task by its type in the registered tasks.
// Returns a pointer to the task if found, nullptr otherwise.
const Task* TaskManager::findTask(TaskType type) const
{
    for (uint8_t i = 0; i < taskCount; i++)
    {
        if (tasks[i].type == type)
        {
            return &tasks[i];
        }
    }

    return nullptr;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Gets the next required item for the currently active task that is not present in the inventory.
// Returns the first required item that is not present in the inventory.
// Returns ItemType::Unknown if no task is active or all required items are present.
ItemType TaskManager::getNextRequiredItem() const
{
    if (!taskActive)
    {
        return ItemType::Unknown;
    }

    const Task* task = findTask(activeTask);

    if (task == nullptr)
    {
        return ItemType::Unknown;
    }

    for (uint8_t i = 0; i < task->itemCount; i++)
    {
        ItemType requiredItem = task->requiredItems[i];

        if (!inventory.hasItem(requiredItem))
        {
            return requiredItem;
        }
    }

    return ItemType::Unknown;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Checks if the currently active task is complete, meaning all required items 
// are present in the inventory.
// Returns true if the task is complete, false otherwise.
bool TaskManager::isTaskComplete() const
{
    if (!taskActive)
    {
        return false;
    }

    const Task* task = findTask(activeTask);

    if (task == nullptr)
    {
        return false;
    }

    for (uint8_t i = 0; i < task->itemCount; i++)
    {
        if (!inventory.hasItem(task->requiredItems[i]))
        {
            return false;
        }
    }

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Checks if any task is currently active.
// Returns true if any task is active, false otherwise.
bool TaskManager::isTaskActive() const
{
    return taskActive;
}