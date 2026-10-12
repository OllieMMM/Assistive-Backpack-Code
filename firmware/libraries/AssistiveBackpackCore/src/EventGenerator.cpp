// EventGenerator.cpp
// Definitions for event generatorion functions.

#include "EventGenerator.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Emit function, stores an event to be used
// Retunrs nothing, updates private data, lastEvent and eventAvailable
bool EventGenerator::emit(const BackpackEvent& event)
{
    if (eventCount >= MAX_EVENTS)
    {
        return false;
    }

    events[eventCount] = event;
    eventCount++;

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Gets the next event stored in the generator, Implamented usinG FIFO queue
// Returns a BackpackEvent struct
BackpackEvent EventGenerator::getEvent()
{
    BackpackEvent event = events[0];

    for (uint8_t i = 1; i < eventCount; i++)
    {
        events[i - 1] = events[i];
    }

    eventCount--;

    return event;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Check if an event is available in the generator
bool EventGenerator::hasEvent() const
{
    return eventCount > 0;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Sets eventAvailable = false
void EventGenerator::clear()
{
    eventCount = 0;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

