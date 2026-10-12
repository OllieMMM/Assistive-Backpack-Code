// Debug Console implamentation file
// This file handles a complete software command interface for testing the backpack.
// The functions where implamented by ChatGPT and then modified and tested by the author.
// This gives an understanding and overview of the capabilities of the backpack core system.
// The system can be adapted to run on an arduino with specific UI and inputs

#include "DebugConsole.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Anonymous namespace for internal helper functions and naming constants.
namespace
{
    const char* itemName(ItemType type)
    {
        switch (type)
        {
        case ItemType::Unknown:     return "Unknown";
        case ItemType::Wallet:      return "Wallet";
        case ItemType::Laptop:      return "Laptop";
        case ItemType::Phone:       return "Phone";
        case ItemType::Keys:        return "Keys";
        case ItemType::Charger:     return "Charger";
        case ItemType::WaterBottle: return "Water bottle";
        }
        return "Invalid item";
    }

    const char* taskName(TaskType type)
    {
        switch (type)
        {
        case TaskType::None:       return "None";
        case TaskType::University: return "University";
        case TaskType::Gym:        return "Gym";
        case TaskType::Work:       return "Work";
        }
        return "Invalid task";
    }

    const char* stateName(BackpackState state)
    {
        switch (state)
        {
        case BackpackState::IDLE:               return "IDLE";
        case BackpackState::CHECKING_INVENTORY: return "CHECKING_INVENTORY";
        case BackpackState::ACQUIRING_ITEMS:    return "ACQUIRING_ITEMS";
        case BackpackState::TASK_REMINDER:      return "TASK_REMINDER";
        }
        return "Unknown state";
    }

    const char* eventName(EventType type)
    {
        switch (type)
        {
        case EventType::Unknown:          return "Unknown";
        case EventType::TaskStarted:      return "TaskStarted";
        case EventType::TaskStopped:      return "TaskStopped";
        case EventType::TaskComplete:     return "TaskComplete";
        case EventType::ItemRequired:     return "ItemRequired";
        case EventType::ItemAcquired:     return "ItemAcquired";
        case EventType::ItemAlreadyPresent:return "ItemAlreadyPresent";
        case EventType::ItemNotRequired:  return "ItemNotRequired";
        case EventType::UnknownRFID:      return "UnknownRFID";
        case EventType::ItemAdded:        return "ItemAdded";
        case EventType::ItemRemoved:      return "ItemRemoved";
        case EventType::TaskReminder:     return "TaskReminder";
        }
        return "Invalid event";
    }

    bool parseChoice(const std::string& text, int& value)
    {
        std::istringstream input(text);
        char extra = '\0';

        if (!(input >> value))
        {
            return false;
        }

        // Reject entries such as "2abc".
        if (input >> extra)
        {
            return false;
        }

        return true;
    }

    bool parseHexTag(std::string text, uint32_t& tagID)
    {
        if (text.empty() || text[0] == '-')
        {
            return false;
        }

        if (text.size() >= 2 &&
            text[0] == '0' &&
            (text[1] == 'x' || text[1] == 'X'))
        {
            text.erase(0, 2);
        }

        if (text.empty())
        {
            return false;
        }

        // Parse explicitly as base 16, not decimal.
        std::istringstream input(text);
        unsigned long long value = 0;

        input >> std::hex >> value;

        if (input.fail() || !input.eof() ||
            value > std::numeric_limits<uint32_t>::max())
        {
            return false;
        }

        tagID = static_cast<uint32_t>(value);
        return true;
    }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Generate the debug console with references to the backpack and inventory objects.
DebugConsole::DebugConsole(Backpack& backpack, Inventory& inventory)
    : backpack(backpack), inventory(inventory)
{
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Prints the debug menu to the console.
void DebugConsole::printMenu() const
{
    std::cout << "\n========== BACKPACK DEBUG CONSOLE ==========\n"
              << "1. Scan an RFID tag\n"
              << "2. Add an item to the backpack\n"
              << "3. Remove an item from the backpack\n"
              << "4. Start a task\n"
              << "5. Stop the active task\n"
              << "6. Display inventory\n"
              << "7. Display backpack/task status\n"
              << "8. Inspect and drain queued events\n"
              << "9. Register an RFID tag mapping\n"
              << "0. Exit\n"
              << "============================================\n"
              << "Select an option: ";
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Helper function to read an RFID tag from the terminal / user input
bool DebugConsole::readTagID(uint32_t& tagID)
{
    std::string input;

    std::cout << "Enter RFID tag ID in hexadecimal "
              << "(for example, 1234 or 0x1234): ";

    if (!std::getline(std::cin, input))
    {
        return false;
    }

    if (!parseHexTag(input, tagID))
    {
        std::cout << "Invalid tag ID. Enter 1-8 hexadecimal digits.\n";
        return false;
    }

    return true;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Helper function to read an item type from the terminal / user input
bool DebugConsole::readItemType(ItemType& type)
{
    int choice = 0;

    std::cout << "\nChoose item type:\n"
              << "1. Wallet\n"
              << "2. Laptop\n"
              << "3. Phone\n"
              << "4. Keys\n"
              << "5. Charger\n"
              << "6. Water bottle\n"
              << "Item type: ";

    std::string input;
    if (!std::getline(std::cin, input) ||
        !parseChoice(input, choice))
    {
        std::cout << "Invalid selection.\n";
        return false;
    }

    switch (choice)
    {
    case 1: type = ItemType::Wallet;      return true;
    case 2: type = ItemType::Laptop;      return true;
    case 3: type = ItemType::Phone;       return true;
    case 4: type = ItemType::Keys;        return true;
    case 5: type = ItemType::Charger;     return true;
    case 6: type = ItemType::WaterBottle; return true;
    default:
        std::cout << "Choose a number from 1 to 6.\n";
        return false;
    }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Helper function to read a task type from the terminal / user input
bool DebugConsole::readTaskType(TaskType& type)
{
    int choice = 0;

    std::cout << "\nChoose task:\n"
              << "1. University\n"
              << "2. Gym\n"
              << "3. Work\n"
              << "Task: ";

    std::string input;
    if (!std::getline(std::cin, input) ||
        !parseChoice(input, choice))
    {
        std::cout << "Invalid selection.\n";
        return false;
    }

    switch (choice)
    {
    case 1: type = TaskType::University; return true;
    case 2: type = TaskType::Gym;        return true;
    case 3: type = TaskType::Work;       return true;
    default:
        std::cout << "Choose a number from 1 to 3.\n";
        return false;
    }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Helper function to print out the complete curernt states and additional 
// information about the backpack system.
void DebugConsole::printStatus() const
{
    std::cout << "\n========== SYSTEM STATUS ==========\n"
              << "Backpack state: "
              << stateName(backpack.getState()) << '\n'
              << "Current task: "
              << taskName(backpack.getCurrentTask()) << '\n'
              << "Task active: "
              << (backpack.isTaskActive() ? "Yes" : "No") << '\n'
              << "Task complete: "
              << (backpack.isTaskComplete() ? "Yes" : "No") << '\n'
              << "Registered inventory entries: "
              << static_cast<int>(inventory.getItemCount()) << '\n';

    if (backpack.isTaskActive() &&
        !backpack.isTaskComplete())
    {
        std::cout << "Next required item: "
                  << itemName(backpack.getNextRequiredItem())
                  << '\n';
    }

    std::cout << "===================================\n";
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Prints all the events in the backpack's event queue, the events are consumed. 
void DebugConsole::printEvents()
{
    if (!backpack.hasEvent())
    {
        std::cout << "No queued events.\n";
        return;
    }

    std::cout << "\nQueued events (oldest first):\n";

    int count = 0;

    while (backpack.hasEvent())
    {
        // Safe because hasEvent() was checked immediately above.
        BackpackEvent event = backpack.getEvent();

        std::cout << ++count
                  << ". " << eventName(event.type)
                  << " | item: " << itemName(event.item)
                  << " | task: " << taskName(event.task)
                  << '\n';
    }

    std::cout << "Event queue drained. These events have been consumed.\n";
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Runs the debug console, providing a command-line interface for interacting with the backpack system.
void DebugConsole::run()
{
    std::cout << "\nAssistive Backpack - Phase 7 Debug Console\n"
              << "Tag IDs are interpreted as hexadecimal.\n";

    bool running = true;

    while (running)
    {
        printMenu();

        std::string input;
        if (!std::getline(std::cin, input))
        {
            std::cout << "\nInput closed. Exiting console.\n";
            break;
        }

        int choice = -1;

        if (!parseChoice(input, choice))
        {
            std::cout << "Please enter a valid menu number.\n";
            continue;
        }

        uint32_t tagID = 0;

        switch (choice)
        {
        case 1:
            if (readTagID(tagID))
            {
                backpack.itemScanned(tagID);
                std::cout << "Scan processed. A scan does not add or remove "
                             "the item automatically.\n";
            }
            break;

        case 2:
            if (readTagID(tagID))
            {
                backpack.itemAdded(tagID);
                std::cout << "Add operation requested.\n";
            }
            break;

        case 3:
            if (readTagID(tagID))
            {
                backpack.itemRemoved(tagID);
                std::cout << "Remove operation requested.\n";
            }
            break;

        case 4:
        {
            TaskType type;

            if (readTaskType(type))
            {
                if (backpack.startTask(type))
                {
                    std::cout << taskName(type) << " task started.\n";
                }
                else
                {
                    std::cout << "Task could not start. Check that it is "
                                 "registered and no task is active.\n";
                }
            }
            break;
        }

        case 5:
            if (backpack.isTaskActive())
            {
                backpack.stopTask();
                std::cout << "Task stopped.\n";
            }
            else
            {
                std::cout << "There is no active task to stop.\n";
            }
            break;

        case 6:
            inventory.printInventory();
            break;

        case 7:
            printStatus();
            break;

        case 8:
            printEvents();
            break;

        case 9:
        {
            ItemType type;

            if (readTagID(tagID) && readItemType(type))
            {
                if (inventory.registerItem(tagID, type))
                {
                    std::cout << "RFID mapping registered: 0x"
                              << std::hex << std::uppercase << tagID
                              << std::dec << " -> " << itemName(type)
                              << '\n';
                }
                else
                {
                    std::cout << "Registration failed. The tag may already "
                                 "exist or inventory capacity may be full.\n";
                }
            }
            break;
        }

        case 0:
            running = false;
            std::cout << "Exiting debug console.\n";
            break;

        default:
            std::cout << "Unknown option. Choose 0 to 9.\n";
            break;
        }
    }
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
