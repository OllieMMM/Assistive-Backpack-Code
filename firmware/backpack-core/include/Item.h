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
    Unknown,
    Wallet,
    Laptop, 
    Phone,
    Keys,
    Charger,
    WaterBottle
};

enum class Location
{
    Unknown,
    Outside,
    Backpack
};

struct Item
{
    uint32_t tagID;
    ItemType type;
    Location location;
};