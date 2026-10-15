// Item.h
// This header file defines the structure and types for items in the Backpack 
// Core project.
// Defines what types of items exist, Where an itm can be located and what 
// information describes on physical item.


#pragma once

#include "Config.h"
#include <stdint.h>

// Enum creates a new data type. Can now write, ItemType item = ItemType::Phone.
enum class ItemType
{
    Unknown,     // = 0
    Wallet,      // = 1
    Laptop,      // = 2
    Phone,       // = 3
    Keys,        // = 4
    Charger,     // = 5
    WaterBottle, // = 6
    Medication,  // = 7
    Glasses,     // = 8
    Umbrella,    // = 9
    Book,        // = 10
    Headphones,  // = 11
    Diary,       // = 12
    // Add more item types as needed.    
};

enum class Location
{
    Unknown, // = 0
    Outside, // = 1
    Backpack // = 2
};

struct RFIDTagID
{
    uint8_t bytes[MAX_RFID_UID_BYTES];
    uint8_t length;
};

// This function compares two RFIDTagID structures to determine if they are the same.
inline bool sameTagID(const RFIDTagID& a, const RFIDTagID& b)
{
    if (a.length != b.length)
    {
        return false;
    }

    if (a.length == 0 || a.length > MAX_RFID_UID_BYTES)
    {
        return false;
    }

    for (uint8_t i = 0; i < a.length; ++i)
    {
        if (a.bytes[i] != b.bytes[i])
        {
            return false;
        }
    }

    return true;
}


struct Item
{
    RFIDTagID tagID;
    ItemType type;
    Location location;
};