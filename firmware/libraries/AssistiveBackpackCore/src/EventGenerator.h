// EventGenerator.h
// Defines the event generator class and holds the event structs that come 
// from the FSM

#pragma once

#include "Events.h"
#include "Config.h"

class EventGenerator
{
public:

    bool emit(const BackpackEvent& event); // Something happened store event

    bool hasEvent() const; // Get the most recently generated event
 
    BackpackEvent getEvent(); // Is there currently an event waiting

    void clear(); // Clear the events and consider the done

private:

    BackpackEvent events[MAX_EVENTS];
    uint8_t eventCount = 0;
};