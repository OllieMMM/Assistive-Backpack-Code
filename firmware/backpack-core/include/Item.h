#pragma once

#include <cstdint>

// Enum creates a new data type. Can now write, ItemType item = ItemType::Phone.
enum class ItemType
{
    Wallet,
    Keys,
    Phone,
    Laptop,
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