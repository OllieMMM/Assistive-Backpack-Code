// This file currently houses all of the main system tests and features 
// implamented to try and catch errors and check performance.

#include <iostream>

#include "Config.h"
#include "Task.h"
#include "Events.h"
#include "Inventory.h"
#include "TaskManager.h"
#include "Backpack.h"
#include "EventGenerator.h"

const char* stateToString(BackpackState state);
void printEvent(const BackpackEvent& event);

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

    // add item that hasnt been registered:
    // Should return false
    std::cout << "\n Adding an unregistered item:"
              << inventory.addItem(0x0001)
              << std::endl;

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

int main_backpack_test()
{
    std::cout << "======== BACKPACK TEST ========" << std::endl;

    Inventory inventory;
    Backpack backpack(inventory);

    backpack.begin();

    std::cout << "Initial state: "
              << stateToString(backpack.getState())
              << '\n';

    Item phone;

    phone.tagID = 0x1234;
    phone.type = ItemType::Phone;
    phone.location = Location::Unknown;

    inventory.registerItem(0x1234, ItemType::Phone);
    inventory.addItem(0x1234);

    // This tests if an item can be added to the inventory without the backpack
    // active.
    inventory.printInventory();

    std::cout << "Initial state: "
              << stateToString(backpack.getState())
              << '\n';

    // Try to start a task before it is registered?
    // Does not work but currently we don't have a way to register tasks!
    // The taskmanager is a private class of the TaskManager.
    bool started = backpack.startTask(TaskType::University);

    std::cout << "Task started: "
          << (started ? "YES" : "NO")
          << '\n';

    std::cout << "State after startTask: "
          << stateToString(backpack.getState())
          << '\n';

    
    // Initialize a univeristy task with required items and register it with the task manager.
    Task university;

    university.type = TaskType::University;

    university.requiredItems[0] = ItemType::Wallet;
    university.requiredItems[1] = ItemType::Keys;
    university.requiredItems[2] = ItemType::Phone;
    university.requiredItems[3] = ItemType::Laptop;

    university.itemCount = 4;

    // register the task and try again 
    backpack.registerTask(university);
    started = backpack.startTask(TaskType::University);

    std::cout << "Task started after registry: "
          << (started ? "YES" : "NO")
          << '\n';

    std::cout << "State after startTask: "
          << stateToString(backpack.getState())
          << '\n';

    // Initialize all of the univeristy items with tags.
    inventory.registerItem(0x0001, ItemType::Wallet);
    inventory.registerItem(0x0002, ItemType::Keys);
    inventory.registerItem(0x0003, ItemType::Laptop);
    
    backpack.itemAdded(0x0001);

    std::cout << "Next required item: " << std::endl;
    std::cout << static_cast<int>(backpack.getNextRequiredItem()) << std::endl;

    std::cout << "State after adding Wallet: "
          << stateToString(backpack.getState())
          << '\n';

    // Note that the items can be added via inventory.addItem but this does 
    // not trigger the checkInventory and FSM transitions! Do not use
    backpack.itemAdded(0x0003);

    std::cout << "State after adding Laptop: "
          << stateToString(backpack.getState())
          << '\n';

    
    std::cout << "Invenotry after adding items above: " << std::endl;
    inventory.printInventory();

    // Next test is to try to start another task while a first is running.
    // Init a new task:
    Task gym;

    gym.type = TaskType::Gym;
    gym.requiredItems[0] = ItemType::Wallet;
    gym.requiredItems[1] = ItemType::Keys;
    gym.requiredItems[2] = ItemType::Phone;
    gym.requiredItems[3] = ItemType::WaterBottle;
    gym.itemCount = 4;

    // register the task
    backpack.registerTask(gym);

    std::cout << "Task University started: "
          << (started ? "YES" : "NO")
          << '\n';

    std::cout << "State: "
          << stateToString(backpack.getState())
          << '\n';

    started = backpack.startTask(TaskType::Gym);

    // try to start task B gym
    std::cout << "Task 'Gym' started: "
          << (started ? "YES" : "NO")
          << '\n';

    std::cout << "State after Task B attempt: "
          << stateToString(backpack.getState())
          << '\n';

    backpack.startTask(TaskType::Gym);

    std::cout << "Current backpack task: " << std::endl;
    std::cout << static_cast<int>(backpack.getCurrentTask()) << std::endl;

    // esting stop Task
    backpack.stopTask();

    std::cout << "State after stopTask: "
          << stateToString(backpack.getState())
          << '\n';

    // Now start task B -> Gym
    bool startedAgain = backpack.startTask(TaskType::Gym);

    std::cout << "Task restarted: "
              << (startedAgain ? "YES" : "NO")
              << '\n';

    std::cout << "State after restart: "
              << stateToString(backpack.getState())
              << '\n';

    inventory.printInventory();

    backpack.itemAdded(0x0002);

    std::cout << "Next required item: " << std::endl;
    std::cout << static_cast<int>(backpack.getNextRequiredItem()) << std::endl;

    backpack.stopTask();

    // Testing a start task with all items required already in backpack.
    backpack.startTask(TaskType::University);
    std::cout << "State after resetting to Univeristy: "
              << stateToString(backpack.getState())
              << '\n';

    backpack.stopTask();

    // Add a secondary wallet
    inventory.registerItem(0x0005, ItemType::Wallet);

    // Adding and removing items
    // backpack.itemAdded(0x0005);

    backpack.itemRemoved(0x0001);
    backpack.itemRemoved(0x0003);

    inventory.printInventory();

    return 0;
}

int main_event_generator_test()
{
    // Test the event generator class induviduully
    EventGenerator generator;

    std::cout << "\n===== EVENT GENERATOR TEST =====\n";

    std::cout << "Initially has event: "
              << generator.hasEvent() << '\n';

    BackpackEvent event;

    event.type = EventType::ItemAcquired;
    event.item = ItemType::Phone;
    event.task = TaskType::University;

    generator.emit(event);

    std::cout << "After emit: "
              << generator.hasEvent() << '\n';

    BackpackEvent received = generator.getEvent();

    std::cout << "Event received.\n";

    generator.clear();

    std::cout << "After clear: "
              << generator.hasEvent() << '\n';

    // GENERATE A BACKPACK CLASS AND SEE THE EVENT GENERATOR BEHAVIOUR
    std::cout << "\n===== BACKPACK EVENT TEST =====\n";
    Inventory inventory;
    Backpack backpack(inventory);

    backpack.begin();

    Task gym;

    gym.type = TaskType::Gym;
    gym.requiredItems[0] = ItemType::Wallet;
    gym.requiredItems[1] = ItemType::Keys;
    gym.requiredItems[2] = ItemType::Phone;
    gym.requiredItems[3] = ItemType::WaterBottle;

    gym.itemCount = 4;

    // register the task
    backpack.registerTask(gym);

    backpack.startTask(TaskType::Gym);

    std::cout <<"Events triggered: " << std::endl;
    while(backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }
    std::cout << "" << std::endl;

    inventory.registerItem(0x0001, ItemType::Wallet);
    inventory.registerItem(0x0002, ItemType::Phone);
    inventory.registerItem(0x0003, ItemType::Keys);
    inventory.registerItem(0x0004, ItemType::WaterBottle);
    inventory.registerItem(0x0005, ItemType::Laptop);

    backpack.itemAdded(0x0001);

    std::cout <<"Events triggered: " << std::endl;
    while(backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }
    std::cout << "" << std::endl;

    backpack.itemAdded(0x0005);
    
    std::cout <<"Events triggered: " << std::endl;
    while(backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }
    std::cout << "" << std::endl;

    backpack.itemAdded(0x2);
    backpack.itemAdded(0x3);
    backpack.itemRemoved(0x4);

    std::cout <<"Events triggered: " << std::endl;
    while(backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }
    std::cout << "" << std::endl;

    backpack.itemScanned(0x0015);
    backpack.itemAdded(0x4);
    backpack.itemAdded(0x15);

    backpack.stopTask();

    std::cout <<"Events triggered: " << std::endl;
    while(backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }
    std::cout << "" << std::endl;

    return 0;
}

// Phase 6 RFID Simulation test harness
int main_rfid_simulation_test()
{
    Inventory inventory;
    Backpack backpack(inventory);

    backpack.begin();

    std::cout << "\n===== RFID SIMULATION TEST =====\n";

    uint32_t knownTag = 0x12345678;
    uint32_t unknownTag = 0x87654321;

    // Register the knwon tag.
    inventory.registerItem(knownTag, ItemType::Keys);
    
    backpack.itemScanned(knownTag);

    while (backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }

    backpack.itemScanned(unknownTag);

    while (backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }

    Task gym;

    gym.type = TaskType::Gym;
    gym.requiredItems[0] = ItemType::Wallet;
    gym.requiredItems[1] = ItemType::Keys;
    gym.requiredItems[2] = ItemType::Phone;
    gym.requiredItems[3] = ItemType::WaterBottle;

    gym.itemCount = 4;

    // register the task
    backpack.registerTask(gym);

    backpack.startTask(TaskType::Gym);

    // Now with a started task the item can be added
    backpack.itemAdded(knownTag);

    while (backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }

    backpack.itemAdded(unknownTag);

    while (backpack.hasEvent())
    {
        printEvent(backpack.getEvent());
    }


    return 0;
}

int main()
{
    // Uncomment the desired test to run.
    // main_data_test();
    // main_inventory_test();
    // main_task_engine();
    // main_backpack_test();
    // main_event_generator_test();
    main_rfid_simulation_test();

    return 0;
}

// State readibility helper function
const char* stateToString(BackpackState state)
{
    switch (state)
    {
        case BackpackState::IDLE:
            return "IDLE";

        case BackpackState::CHECKING_INVENTORY:
            return "CHECKING_INVENTORY";

        case BackpackState::ACQUIRING_ITEMS:
            return "ACQUIRING_ITEMS";

        case BackpackState::TASK_REMINDER:
            return "TASK_REMINDER";
    }

    return "UNKNOWN";
}

// Helper funcion for event readibility
const char* eventTypeToString(EventType type)
{
    switch (type)
    {
        case EventType::TaskStarted:
            return "TASK_STARTED";

        case EventType::TaskStopped:
            return "TASK_STOPPED";

        case EventType::TaskComplete:
            return "TASK_COMPLETE";

        case EventType::ItemRequired:
            return "ITEM_REQUIRED";

        case EventType::ItemAcquired:
            return "ITEM_ACQUIRED";

        case EventType::ItemNotRequired:
            return "ITEM_NOT_REQUIRED";

        case EventType::UnknownRFID:
            return "UNKNOWN_RFID";

        case EventType::ItemAdded:
            return "ITEM_ADDED";

        case EventType::ItemRemoved:
            return "ITEM_REMOVED";

        case EventType::TaskReminder:
            return "TASK_REMINDER";
    }

    return "UNKNOWN";
}

void printEvent(const BackpackEvent& event)
{     
    std::cout << "Event: "
              << eventTypeToString(event.type)
              << '\n';
}   

