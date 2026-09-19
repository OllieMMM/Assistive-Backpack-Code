#include <iostream>

#include "Config.h"
#include "Task.h"
#include "Events.h"
#include "Inventory.h"
#include "TaskManager.h"

// Phase 1 Test code for Data Model
int main_data_test()
{
    // Create a physical item
    Item phone;

    phone.tagID = 0x1242;
    phone.type = ItemType::Phone;
    phone.location = Location::Unknown;

    // Create a task
    Task universityTask;

    universityTask.type = TaskType::University;

    universityTask.requiredItems[0] = ItemType::Wallet;
    universityTask.requiredItems[1] = ItemType::Keys;
    universityTask.requiredItems[2] = ItemType::Phone;
    universityTask.requiredItems[3] = ItemType::Laptop;
    universityTask.requiredItems[4] = ItemType::Charger;

    universityTask.itemCount = 5;
    universityTask.reminderAudio = 1;

    // Create an event
    // This will be based and triggered by the UI
    BackpackEvent event;

    event.type = EventType::ItemAcquired;
    event.item = ItemType::Phone;
    event.task = TaskType::University;

    std::cout << "=== Backpack Data Model Test ===" << std::endl;

    std::cout << "Phone RFID: 0x"
              << std::hex << phone.tagID << std::endl;

    std::cout << std::dec;

    std::cout << "University task items: "
              << static_cast<int>(universityTask.itemCount)
              << std::endl;

    std::cout << "Data model test successful!" << std::endl;

    return 0;
}

// Phase 2 Test code for Inventory Manager
int main_inventory_test()
{
    Inventory inventory;

    std::cout << "=== INVENTORY TEST ===" << std::endl;

    // Register known physical items.
    std::cout << "\nRegistering items..." << std::endl;

    std::cout << "Wallet: "
              << inventory.registerItem(0x1234, ItemType::Wallet)
              << std::endl;

    std::cout << "Keys: "
              << inventory.registerItem(0x5678, ItemType::Keys)
              << std::endl;

    std::cout << "Phone: "
              << inventory.registerItem(0xABCD, ItemType::Phone)
              << std::endl;

    // Add wallet and keys to backpack.
    std::cout << "\nAdding wallet and keys..." << std::endl;

    inventory.addItem(0x1234);
    inventory.addItem(0x5678);

    // Check inventory.
    std::cout << "\nInventory status:" << std::endl;

    std::cout << "Wallet available: "
              << inventory.hasItem(ItemType::Wallet)
              << std::endl;

    std::cout << "Keys available: "
              << inventory.hasItem(ItemType::Keys)
              << std::endl;

    std::cout << "Phone available: "
              << inventory.hasItem(ItemType::Phone)
              << std::endl;

    // Check RFID lookup.
    std::cout << "\nRFID lookup:" << std::endl;

    std::cout << "0x1234 is registered: "
              << (inventory.findItem(0x1234) != nullptr)
              << std::endl;

    std::cout << "0x9999 is registered: "
              << (inventory.findItem(0x9999) != nullptr)
              << std::endl;

    // Remove wallet.
    std::cout << "\nRemoving wallet..." << std::endl;

    inventory.removeItem(0x1234);

    std::cout << "Wallet available after removal: "
              << inventory.hasItem(ItemType::Wallet)
              << std::endl;

    // Unknown tag.
    std::cout << "\nUnknown tag test:" << std::endl;

    std::cout << "Unknown RFID type: "
              << static_cast<int>(inventory.getItemType(0x9999))
              << std::endl;

    // Duplicate registration test.
    std::cout << "\nDuplicate registration test: "
              << inventory.registerItem(0x1234,ItemType::Wallet)
              << std::endl;
    
    // Exceeding max items test.
    std::cout << "\nExceeding max items test:" << std::endl;
    for (uint8_t i = 0; i < MAX_ITEMS + 1; i++)
    {
        std::cout << "Registering item " << static_cast<int>(i) << ": "
                  << inventory.registerItem(0x1000 + i, ItemType::Unknown)
                  << std::endl;
    }
    
    // Print inventory.
    std::cout << std::endl;

    inventory.printInventory();

    inventory.clearInventory();

    std::cout << std::endl;
    inventory.printInventory();

    return 0;
}

int main_task_engine()
{
    std::cout << "\n===== TASK TEST =====\n";

    // Generate an inventory and task manager
    Inventory inventory;

    TaskManager taskManager(inventory);

    // Initialize a univeristy task with required items and register it with the task manager.
    Task university;

    university.type = TaskType::University;

    university.requiredItems[0] = ItemType::Wallet;
    university.requiredItems[1] = ItemType::Keys;
    university.requiredItems[2] = ItemType::Phone;
    university.requiredItems[3] = ItemType::Laptop;

    university.itemCount = 4;

    // Testing Task Manager functionality and private data
    bool registered = taskManager.registerTask(university);

    std::cout << "Task registered: "
              << registered
              << std::endl;

    bool started = taskManager.startTask(TaskType::University);

    std::cout << "Task started: "
              << started
              << std::endl;

    std::cout << "Task active: "
              << taskManager.isTaskActive()
              << std::endl;

    std::cout << "Next required item: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              << std::endl;
    
    // Testing with inventory and registering of physical items.
    // Register 4 items in the inventory.              
    inventory.registerItem(0x1234, ItemType::Wallet);
    inventory.registerItem(0x5678, ItemType::Keys);
    inventory.registerItem(0xABCD, ItemType::Phone);
    inventory.registerItem(0xDEAD, ItemType::Laptop);
    
    // Add 2 items to the backapck, leaving 2 required items missing.
    inventory.addItem(0x1234);
    inventory.addItem(0x5678);
    std::cout << "2 Items added sucessfully to the backpack." << std::endl;

    // Check the item got added and the next required item is correct.
    std::cout << "Next required item after adding wallet and keys: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              << std::endl;
    
    inventory.addItem(0xABCD);
    std::cout << "1 Item added sucessfully to the backpack." << std::endl;

    std::cout << "Next required item after adding Phone: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              << std::endl;

    inventory.addItem(0xDEAD);
    std::cout << "1 Item added sucessfully to the backpack." << std::endl;
    
    std::cout << "Task complete: "
              << taskManager.isTaskComplete()
              << std::endl;

    std::cout << "Next required item after adding Laptop: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              << std::endl;

    taskManager.stopTask();

    std::cout << "Task active after stopping: "
              << taskManager.isTaskActive()
              << std::endl;
    
    std::cout << "Next required item after adding wallet and keys: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              << std::endl;

    std::cout << "Current task after stopping: "
              << static_cast<int>(taskManager.getCurrentTask())
              << std::endl;
    
    // Edge cases and other debugs
    std::cout << "\n===== TASK TEST EDGE CASES =====" << std::endl;

    // Task start without registration
    std::cout << "Starting unregistered task: "
              << taskManager.startTask(TaskType::Gym)
              << std::endl;

    // Registration of two tasks of the same type
    std::cout << "Registering a duplicate University task: "  
              << taskManager.registerTask(university)
              << std::endl;

    //clear the backpack inventory and test the next required item when no items are present.
    inventory.clearInventory();

    // Complete dual item scanning and task updates
    std::cout << "Adding two wallets and checking next required item: "
              << inventory.registerItem(0x1111, ItemType::Wallet)
              << " "
              << inventory.registerItem(0x2222, ItemType::Wallet)
              << std::endl;

    std::cout << "Registered two wallets to inventory:\n"
              << "Starting University task again: "
              << taskManager.startTask(TaskType::University)
              << "\nCurrent task: "
              << static_cast<int>(taskManager.getCurrentTask())
              << "\nAdded first wallet to backpack: "
              << inventory.addItem(0x1111)
              << "\nNext item required: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              <<std::endl;

    inventory.registerItem(0x3333, ItemType::WaterBottle);
    inventory.addItem(0x3333);

    std::cout << "Added water bottle to backpack" << std::endl;

    std::cout << "Next item required: "
              << static_cast<int>(taskManager.getNextRequiredItem())
              << std::endl;

    return 0;
}

int main()
{
    // Uncomment the desired test to run.

    // return main_data_test();
    // return main_inventory_test();
    return main_task_engine();

}