// Backpack.cpp
// This source file implements the functionality of the Backpack class.

#include "Backpack.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Backpack constructor initializes the Backpack with a reference to an Inventory 
// object and initializes the TaskManager with the same Inventory reference.
Backpack::Backpack(Inventory& inventory)
    : inventory(inventory),
      taskManager(inventory)
{
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Initializes the Backpack system, preparing it for operation.
// Sets the initial state of the Backpack to IDLE.
void Backpack::begin()
{
    state = BackpackState::IDLE;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Returns the current state of the Backpack.
BackpackState Backpack::getState() const
{
    return state;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Start a task of the specified type using the TaskManager.
// Returns true if the task was successfully started, false otherwise.
bool Backpack::startTask(TaskType type)
{
    // If the backpack is not in the IDLE state, we cannot start a new task.
    if (state != BackpackState::IDLE)
    {
        return false;
    }

    // Attempt to start the task with the specified type.
    if (!taskManager.startTask(type))
    {
        // If the task could not be started 
        // (e.g., it is not registered, or another task is active), return false.
        return false;
    }

    state = BackpackState::CHECKING_INVENTORY;

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Stop the currently active task using the TaskManager.
// Resets the Backpack state to IDLE.
void Backpack::stopTask()
{
    taskManager.stopTask();
    state = BackpackState::IDLE;
}