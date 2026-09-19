#pragma once

#include <cstdint>

#include "Config.h"
#include "Item.h"

class Inventory
{
public:
    // Accessable from outside the class, functions can be called from other files.

    void clearInventory();

    bool registerItem(uint32_t tagID, ItemType type);

    bool addItem(uint32_t tagID);

    bool removeItem(uint32_t tagID);

    bool hasItem(ItemType type) const; //The const keyword indicates that this function does not modify the state of the object.

    Item* findItem(uint32_t tagID);

    ItemType getItemType(uint32_t tagID) const;

    void printInventory() const;

private:
    // Accessable only from within the class, internal data and methods.

    Item items[MAX_ITEMS];
    uint8_t itemCount = 0;
};