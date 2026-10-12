// Debug consle header file
// This file contains function declaritions for the DebugConsle
// Used to implament and test the backpack core system. 
// This is a command line interface that allows
#pragma once

#include <cstdint>

#include "Backpack.h"
#include "Inventory.h"

class DebugConsole
{
public:
    DebugConsole(Backpack& backpack, Inventory& inventory);
    // Recieves references rather then creating a new backpack and inventory.

    void run();

private:
    Backpack& backpack;
    Inventory& inventory;

    void printMenu() const;
    void printStatus() const;
    void printEvents();

    bool readTagID(uint32_t& tagID);
    bool readItemType(ItemType& type);
    bool readTaskType(TaskType& type);
};
