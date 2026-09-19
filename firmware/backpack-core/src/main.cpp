#include <iostream>

#include "Config.h"
#include "Item.h"
#include "Task.h"
#include "Events.h"

int main()
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