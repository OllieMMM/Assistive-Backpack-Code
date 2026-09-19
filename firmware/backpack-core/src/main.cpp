#include <iostream>

#include "Config.h"
#include "Task.h"
#include "Events.h"
#include "Inventory.h"

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
int main()
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