// Inventory.h
// This header file defines the Inventory class, which manages a collection of items in a backpack inventory system.
// We do not define the implementation of the functions here, only their declarations. 
// The actual implementation will be in the corresponding Inventory.cpp file.

#pragma once

#include <stdint.h>

#include "Config.h"
#include "Item.h"

class Inventory
{
public:
    // Accessable from outside the class, functions can be called from other files.

    void clearInventory();

    uint8_t getItemCount() const; // Returns the current number of items in the inventory.

    Item* getItemAt(uint8_t index);
    
    Item* findItem(const RFIDTagID& tagID);
    
    bool registerItem(const RFIDTagID& tagID, ItemType type);

    bool addItem(const RFIDTagID& tagID);

    bool removeItem(const RFIDTagID& tagID);

    bool hasItem(ItemType type) const; //The const keyword indicates that this function does not modify the state of the object.

    ItemType getItemType(const RFIDTagID& tagID) const;

private:
    // Accessable only from within the class, internal data and methods.

    Item items[MAX_ITEMS];
    uint8_t itemCount = 0;
};