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

#include <Backpack.h>
#include <DebugConsoleArduino.h>

void setup()
{
    Serial.begin(9600);

    Serial.println("Assistive Backpack - Core Library Test");

    // One inventory shared by the backpack and task manager.
    Inventory inventory;
    Backpack backpack(inventory);

    // Intialize and run the backpack
    backpack.begin();
    Serial.println("Backpack core begin() called.");
    
    // Initialize and run the debug console
    DebugConsoleArduino console(backpack, inventory);
    console.begin(9600);
   
}


void loop()
{
    console.update()
}
