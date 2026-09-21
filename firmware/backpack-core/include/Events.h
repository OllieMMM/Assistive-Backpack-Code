// Events.h
// This header file defines the structure and types for events in the Backpack
// More events can be added in future. This is the connection point between
// the firware and the hardware. Passes around and EVENT struct that contains 
// the event type, and any relevant information about the event. This is used by
// the feedback system to produce user feedback.

#pragma once

#include "Item.h"
#include "Task.h"

enum class EventType
{
    Unknown,
    TaskStarted,
    TaskStopped,
    TaskComplete,
    ItemRequired,
    ItemAcquired, // Specific to the task
    ItemAlreadyPresent,
    ItemNotRequired,
    UnknownRFID,
    ItemAdded, // Added to backpack 
    ItemRemoved,
    TaskReminder
};

struct BackpackEvent
{
    // All custom classes, refer to Task.h and Item.h
    EventType type;
    ItemType item;
    TaskType task;
};