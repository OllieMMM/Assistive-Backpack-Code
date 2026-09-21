// Backpack.h
// This header file defines the Backpack class.
// Manages the overall state of the backpack, including inventory and tasks.

#pragma once

#include <cstdint>

#include "Inventory.h"
#include "TaskManager.h"
#include "EventGenerator.h"

// Backpack states
enum class BackpackState
{
    IDLE,
    CHECKING_INVENTORY,
    ACQUIRING_ITEMS,
    TASK_REMINDER
};

// Backpack class manages the overall state of the backpack, including inventory 
// and tasks.
// It interacts with the Inventory and TaskManager and EventGenerator classes 
// to handle item scanning, adding, and removing, as well as task management.
class Backpack
{
public:

    explicit Backpack(Inventory& inventory);

    void begin(); 

    BackpackState getState() const;
    
    bool registerTask(const Task& task);
    bool startTask(TaskType type);
    void stopTask();

    void itemScanned(uint32_t tagID); 
    void itemAdded(uint32_t tagID);
    void itemRemoved(uint32_t tagID);

    TaskType getCurrentTask() const;
    ItemType getNextRequiredItem() const;

    bool isTaskActive() const;
    bool isTaskComplete() const;
    bool isItemRequired(ItemType type) const;

    bool hasEvent() const;
    BackpackEvent getEvent();
    void clearEvents();

private:

    void checkInventory();
    void updateState();

    Inventory& inventory;
    TaskManager taskManager;
    EventGenerator eventGenerator;

    BackpackState state = BackpackState::IDLE;
};

