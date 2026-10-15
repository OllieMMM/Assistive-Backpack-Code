
#include <iostream>

#include "Backpack.h"
#include "DebugConsole.h"
#include "Inventory.h"
#include "Item.h"
#include "Task.h"

int main()
{
    // One inventory shared by the backpack and task manager.
    Inventory inventory;
    Backpack backpack(inventory);

    // Prepare the backpack state machine.
    backpack.begin();

    // Register demo RFID tags.
    // These are hexadecimal IDs, matching the console input convention.
    const bool mappingsRegistered = 
    inventory.registerItem(
        RFIDTagID{{00, 00, 00, 01}, 6},
        ItemType::Wallet) &&

    inventory.registerItem(
        RFIDTagID{{00, 00, 00, 02}, 6},
        ItemType::Keys) &&

    inventory.registerItem(
        RFIDTagID{{00, 00, 00, 03}, 6},
        ItemType::Phone) &&

    inventory.registerItem(
        RFIDTagID{{00, 00, 00, 04}, 6},
        ItemType::Laptop) &&

    inventory.registerItem(
        RFIDTagID{{00, 00, 00, 05}, 6},
        ItemType::Charger) &&

    inventory.registerItem(
        RFIDTagID{{00, 00, 00, 06}, 6},
        ItemType::WaterBottle);

    if (!mappingsRegistered)
    {
        std::cerr << "Failed to register demo RFID mappings.\n";
        return 1;
    }

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
    if (!backpack.registerTask(university) ||
        !backpack.registerTask(gym) ||
        !backpack.registerTask(work))
    {
        std::cerr << "Failed to register demo tasks.\n";
        return 1;
    }

    std::cout << "Demo environment initialized successfully.\n"
              << "Registered tags: 0x1001 to 0x1006\n"
              << "Registered tasks: University, Gym, Work\n";

    // Start the interactive terminal simulation.
    DebugConsole console(backpack, inventory);
    console.run();

    return 0;
}