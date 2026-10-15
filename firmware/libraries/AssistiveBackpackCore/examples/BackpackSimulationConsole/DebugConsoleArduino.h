#pragma once

#include <Arduino.h>

#include "Backpack.h"
#include "Inventory.h"

/*
 * Arduino-specific serial debug console for the Assistive Backpack core.
 *
 * This class belongs in the Arduino application layer, NOT in the
 * hardware-independent AssistiveBackpackCore library.
 *
 * Usage:
 *   DebugConsoleArduino console(backpack, inventory);
 *
 *   void setup()
 *   {
 *       console.begin(9600);
 *   }
 *
 *   void loop()
 *   {
 *       console.update();
 *   }
 *
 * Commands are entered as one line in Serial Monitor:
 *   help
 *   scan 0x1001
 *   add 0x1001
 *   remove 0x1001
 *   start 1
 *   stop
 *   inventory
 *   status
 *   events
 *   register 0x1007 1
 *
 * Numeric ItemType and TaskType values depend on the enums in your core.
 * Check Item.h and Task.h before using the register/start commands.
 */

class DebugConsoleArduino
{
public:
    DebugConsoleArduino(Backpack& backpack, Inventory& inventory);

    // Starts the Serial connection and prints the command help.
    void begin(unsigned long baudRate = 9600);

    // Call repeatedly from loop(). Input handling is non-blocking.
    void update();

private:
    static const uint8_t INPUT_BUFFER_SIZE = 64;

    Backpack& backpack;
    Inventory& inventory;

    char inputBuffer[INPUT_BUFFER_SIZE];
    uint8_t inputLength;

    void printHelp();
    void processCommand(char* line);
    void printInventory();
    void printStatus();
    void printEvents();

    static const char* itemName(ItemType type);
    static const char* locationName(Location location);
    static const char* eventName(EventType type);
    static const char* stateName(BackpackState state);
    static const char* taskName(TaskType type);
    static bool parseUnsignedLong(const char* text, unsigned long& value);
    static bool parseTagID(const char* text, RFIDTagID& tagID);
    static void printTagID(const RFIDTagID& tagID);

};
