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
    TaskStarted,
    TaskStopped,
    TaskComplete,
    ItemRequired,
    ItemAcquired,
    ItemAlreadyPresent,
    ItemNotRequired,
    UnknownRFID,
    ItemAdded,
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