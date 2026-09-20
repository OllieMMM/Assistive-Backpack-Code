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

// Registers a task with the backpacks associated TaskManager
// Returns true if the task registers sucessfully
bool Backpack::registerTask(const Task& task)
{
    return taskManager.registerTask(task);
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

    // After starting the task, immediately check the inventory.
    // If the task is complete, the state will be updated to TASK_REMINDER;
    // otherwise, it will be updated to ACQUIRING_ITEMS.
    checkInventory();
    
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
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Checks the inventory to determine if the current task is complete or 
// if items are still needed.
// Updates the Backpack state accordingly.
void Backpack::checkInventory()
{
    if (taskManager.isTaskComplete())
    {
        // Switch to user reminder state if the task is complete.
        state = BackpackState::TASK_REMINDER;
    }
    else
    {
        state = BackpackState::ACQUIRING_ITEMS;
    }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Add item to the Backpack's inventory when it is scanned.
// This function should be called when an item is scanned into the backpack.
// Takes in the tagID (e.g. RFID Tag = 0x1234) 
void Backpack::itemAdded(uint32_t tagID)
{
    // Inventory updates regardless of backpack state.
    // (ie. user can add waterbottle without having to start a task.)
    inventory.addItem(tagID);

    if (state == BackpackState::ACQUIRING_ITEMS)
    {
        // Transition only if the task is complete and a task is running.
        checkInventory();
    }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Remove item from the backpack (sets this item location flag = outside)
void Backpack::itemRemoved(uint32_t tagID)
{
    inventory.removeItem(tagID);
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Picks up that an item was scanned but not necessarily placed into/out of the 
// backpack. Currently does nothing.
void Backpack::itemScanned(uint32_t tagID)
{
    // RFID/input subsystem integration point.
    // A scan does not automatically mean the item was added.
    // Calls either itemAdded or itemRemoved or perhaps also itemRegister.
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Get the current active task if any
// Helper function, returns the current task type
TaskType Backpack::getCurrentTask() const
{
    return taskManager.getCurrentTask();
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Get the next required item for the currently active task
// Helper function, returns the current item type
ItemType Backpack::getCurrentRequiredItem() const
{
    return taskManager.getNextRequiredItem();
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Check if a task is currently active.
// Returns true if yes.
bool Backpack::isTaskActive() const
{
    return state != BackpackState::IDLE;
    // Could also implament by calling the TaskManager::isTaskActive()
    // return taskManager.isTaskActive();
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Check if the current task has all necessary items and is considered complete
// Returns true if complete
bool Backpack::isTaskComplete() const
{
    return taskManager.isTaskComplete();
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

