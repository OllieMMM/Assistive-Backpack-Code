// Assitive Backpack Code
// This file implaments a custom inventory and event management system
// to work with the Assitive Backpack for our ESI 2026 project.
// More information https://github.com/OllieMMM/Assistive-Backpack-Code
// Written by Oliver McKinlay

/*
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

// Initialize one inventory shared by the backpack and task manager.
Inventory inventory;
Backpack backpack(inventory);
DebugConsoleArduino console(backpack, inventory);

void setup()
{
    Serial.begin(115200);
    Serial.println("Assistive Backpack - Core Library Test");

    // Intialize and run the backpack
    backpack.begin();
    Serial.println("Backpack core begin() called.");
    
    // Register demo RFID tags.
    // These are example hexadecimal IDs.
    inventory.registerItem(RFIDTagID{{00, 00, 00, 01}, 4}, ItemType::Wallet) &&
    inventory.registerItem(RFIDTagID{{00, 00, 00, 02}, 4}, ItemType::Keys) &&
    inventory.registerItem(RFIDTagID{{00, 00, 00, 03}, 4}, ItemType::Phone) &&
    inventory.registerItem(RFIDTagID{{00, 00, 00, 04}, 4}, ItemType::Laptop) &&
    inventory.registerItem(RFIDTagID{{00, 00, 00, 05}, 4}, ItemType::Charger) &&
    inventory.registerItem(RFIDTagID{{00, 00, 00, 06}, 4}, ItemType::WaterBottle);

    // University task: laptop, phone and keys.
    Task university{};
    university.type = TaskType::University;
    university.requiredItems[0] = ItemType::Laptop;
    university.requiredItems[1] = ItemType::Phone;
    university.requiredItems[2] = ItemType::Keys;
    university.itemCount = 3;
    university.reminderAudio = 1;

    // Gym task: keys, phone and water bottle.
    Task gym{};
    gym.type = TaskType::Gym;
    gym.requiredItems[0] = ItemType::Keys;
    gym.requiredItems[1] = ItemType::Phone;
    gym.requiredItems[2] = ItemType::WaterBottle;
    gym.itemCount = 3;
    gym.reminderAudio = 2;

    // Work task: laptop, phone and charger.
    Task work{};
    work.type = TaskType::Work;
    work.requiredItems[0] = ItemType::Laptop;
    work.requiredItems[1] = ItemType::Phone;
    work.requiredItems[2] = ItemType::Charger;
    work.itemCount = 3;
    work.reminderAudio = 3;

    // Register tasks through Backpack's public interface.
    backpack.registerTask(university);
    backpack.registerTask(gym);
    backpack.registerTask(work);

    Serial.println("Demo environment initialized successfully.");
    Serial.println("Registered tags: 00000001 to 00000006");
    Serial.println("Registered tasks: University, Gym, Work");

    // Begin the debug console, which will handle user input and output.
    console.begin(115200);
}


void loop()
{
    console.update();
}
