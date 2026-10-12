// Item.h
// This header file defines the structure and types for items in the Backpack 
// Core project.
// Defines what types of items exist, Where an itm can be located and what 
// information describes on physical item.


#pragma once

#include <cstdint>

// Enum creates a new data type. Can now write, ItemType item = ItemType::Phone.
enum class ItemType
{
    Unknown,    // = 0
    Wallet,     // = 1
    Laptop,     // = 2
    Phone,      // = 3
    Keys,       // = 4
    Charger,    // = 5
    WaterBottle // = 6
};

enum class Location
{
    Unknown, // = 0
    Outside, // = 1
    Backpack // = 2
};

struct Item
{
    uint32_t tagID;
    ItemType type;
    Location location;
};