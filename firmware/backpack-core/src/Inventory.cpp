// Inventory.cpp
// This source file implements the functionality of the Inventory Manager.

#include <iostream>

#include "Inventory.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Clear the inventory by resetting the item count to zero.
// Refers to the private member variable itemCount. Inventory::itemCount
void Inventory::clearInventory()
{
    itemCount = 0;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Returns the number of items currently registered by inventory.
uint8_t Inventory::getItemCount() const
{
    return itemCount;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Searches for an item in the registered inventory by its tagID.
// If found, returns a pointer to the item; otherwise, returns nullptr.
Item* Inventory::findItem(uint32_t tagID)
{
    for (uint8_t i = 0; i < itemCount; i++)
    {
        if (items[i].tagID == tagID)
        {
            return &items[i]; //Return an address to the item if found.
        }
    }

    return nullptr;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Registers a new item in the inventory with the given tagID and type.
// Returns true if the item was successfully registered, false otherwise.
bool Inventory::registerItem(uint32_t tagID, ItemType type)
{
    // Do not register the same tag twice.
    if (findItem(tagID) != nullptr)
    {
        return false;
    }

    // Do not exceed the inventory capacity.
    if (itemCount >= MAX_ITEMS)
    {
        return false;
    }

    items[itemCount].tagID = tagID;
    items[itemCount].type = type;
    items[itemCount].location = Location::Unknown;

    itemCount++;

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Add item to the BackPack specific inventory.
// Updates an item to be in the specified location (Backpack) based on its tagID.
// Returns true if the item was successfully added, false otherwise.
bool Inventory::addItem(uint32_t tagID)
{
    Item* item = findItem(tagID);

    if (item == nullptr)
    {
        return false;
    }

    item->location = Location::Backpack;

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Remove item from the BackPack specific inventory.
// Updates an item to be in the specified location (Outside) based on its tagID.
// Returns true if the item was successfully removed, false otherwise.
bool Inventory::removeItem(uint32_t tagID)
{
    Item* item = findItem(tagID);

    if (item == nullptr)
    {
        return false;
    }

    item->location = Location::Outside;

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Checks if an item of the specified type is present in the backpack inventory.
// Returns true if the item is found, false otherwise.
// Asks if there is a current item specifically in the backpack, not just 
// registered by the inventory.
bool Inventory::hasItem(ItemType type) const
{
    for (uint8_t i = 0; i < itemCount; i++)
    {
        if (items[i].type == type &&
            items[i].location == Location::Backpack)
        {
            return true;
        }
    }

    return false;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ItemType Inventory::getItemType(uint32_t tagID) const
{
    for (uint8_t i = 0; i < itemCount; i++)
    {
        if (items[i].tagID == tagID)
        {
            return items[i].type;
        }
    }

    return ItemType::Unknown;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Prints the current inventory to the console for debugging purposes.
void Inventory::printInventory() const
{
    std::cout << "===== INVENTORY =====" << std::endl;

    for (uint8_t i = 0; i < itemCount; i++)
    {
        std::cout << "Tag: 0x"
                  << std::hex
                  << items[i].tagID
                  << std::dec
                  << std::endl;

        std::cout << "Item index: "
                  << static_cast<int>(i)
                  << std::endl;
        
        std::cout << "Type: "
                  << static_cast<int>(items[i].type)
                  << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Total items: "
              << static_cast<int>(itemCount)
              << std::endl;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++