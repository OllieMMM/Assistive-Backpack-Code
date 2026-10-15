#include "DebugConsoleArduino.h"

#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Construction and serial setup
// -----------------------------------------------------------------------------

DebugConsoleArduino::DebugConsoleArduino(Backpack& backpackRef,
                                         Inventory& inventoryRef)
    : backpack(backpackRef),
      inventory(inventoryRef),
      inputLength(0)
{
    inputBuffer[0] = '\0';
}

void DebugConsoleArduino::begin(unsigned long baudRate)
{   
    delay(100); // Allow time for the USB serial connection to establish.

    Serial.begin(baudRate);

    // Avoid waiting forever for a USB serial connection: some Arduino boards
    // do not need the host to open Serial before the sketch can continue.
    Serial.println();
    Serial.println(F("Assistive Backpack - Arduino Debug Console"));
    Serial.println(F("Type 'help' to see available commands."));
    printHelp();
}

void DebugConsoleArduino::update()
{
    // Process only characters that have already arrived. This avoids
    // readStringUntil() and its timeout/blocking behaviour.
    while (Serial.available() > 0)
    {
        const char incoming = static_cast<char>(Serial.read());

        if (incoming == '\r' || incoming == '\n')
        {
            if (inputLength > 0)
            {
                inputBuffer[inputLength] = '\0';
                processCommand(inputBuffer);
                inputLength = 0;
                inputBuffer[0] = '\0';
            }
        }
        else if (inputLength < INPUT_BUFFER_SIZE - 1)
        {
            inputBuffer[inputLength++] = incoming;
        }
        else
        {
            // Discard an overlong command instead of overflowing the buffer.
            inputLength = 0;
            inputBuffer[0] = '\0';
            Serial.println(F("Error: command too long; input discarded."));
        }
    }
}

// -----------------------------------------------------------------------------
// Command parsing
// -----------------------------------------------------------------------------

void DebugConsoleArduino::processCommand(char* line)
{
    // strtok edits the input buffer in place, avoiding dynamic String objects.
    char* command = strtok(line, " \t");
    if (command == nullptr)
    {
        return;
    }

    if (strcmp(command, "help") == 0)
    {
        printHelp();
    }
    
    else if (strcmp(command, "scan") == 0)
    {
        char* tagText = strtok(nullptr, " \t");

        RFIDTagID tagID = {};

        if (tagText == nullptr || !parseTagID(tagText, tagID))
        {
            Serial.println(
                F("Usage: scan <UID>, e.g. scan 4A123C4D"));
            Serial.println(
                F("UID must contain 8, 14, or 20 hexadecimal characters."));
            return;
        }

        Serial.print(F("Simulating RFID scan: "));
        printTagID(tagID);
        Serial.println();

        backpack.itemScanned(tagID);

        Serial.println(F("Scan passed to Backpack core."));
    }

    
    else if (strcmp(command, "add") == 0)
    {
        char* tagText = strtok(nullptr, " \t");

        RFIDTagID tagID = {};

        if (tagText == nullptr || !parseTagID(tagText, tagID))
        {
            Serial.println(
                F("Usage: add <UID>, e.g. add 4A123C4D"));
            return;
        }

        backpack.itemAdded(tagID);

        Serial.println(
            F("Add command sent to Backpack core. Use 'inventory' or 'status' to verify."));
    }

    
    else if (strcmp(command, "remove") == 0)
    {
        char* tagText = strtok(nullptr, " \t");

        RFIDTagID tagID = {};

        if (tagText == nullptr || !parseTagID(tagText, tagID))
        {
            Serial.println(
                F("Usage: remove <UID>, e.g. remove 4A123C4D"));
            return;
        }

        backpack.itemRemoved(tagID);

        Serial.println(
            F("Remove command sent to Backpack core. Use 'inventory' or 'status' to verify."));
    }

    else if (strcmp(command, "start") == 0)
    {
        char* taskText = strtok(nullptr, " \t");
        unsigned long taskValue = 0;

        if (taskText == nullptr || !parseUnsignedLong(taskText, taskValue))
        {
            Serial.println(F("Usage: start <numeric TaskType value>"));
            Serial.println(F("Check Task.h for the TaskType enum values."));
            return;
        }

        const bool success = backpack.startTask(static_cast<TaskType>(taskValue));
        Serial.println(success ? F("Task start accepted.")
                               : F("Task start rejected; check task registration/state."));
    }
    else if (strcmp(command, "stop") == 0)
    {
        backpack.stopTask();
        Serial.println(F("Stop-task command sent."));
    }
    else if (strcmp(command, "inventory") == 0)
    {
        printInventory();
    }
    else if (strcmp(command, "status") == 0)
    {
        printStatus();
    }
    else if (strcmp(command, "events") == 0)
    {
        printEvents();
    }
    
    else if (strcmp(command, "register") == 0)
    {
        char* tagText = strtok(nullptr, " \t");
        char* typeText = strtok(nullptr, " \t");

        RFIDTagID tagID = {};
        unsigned long typeValue = 0;

        if (tagText == nullptr ||
            typeText == nullptr ||
            !parseTagID(tagText, tagID) ||
            !parseUnsignedLong(typeText, typeValue))
        {
            Serial.println(
                F("Usage: register <UID> <numeric ItemType value>"));
            Serial.println(
                F("Example: register 4A123C4D 1"));
            Serial.println(
                F("UID must contain 8, 14, or 20 hexadecimal characters."));
            return;
        }

        const bool success = inventory.registerItem(
            tagID,
            static_cast<ItemType>(typeValue));

        Serial.println(
            success
                ? F("RFID mapping registered.")
                : F("Registration failed; tag may already exist or inventory may be full."));
    }

    else
    {
        Serial.print(F("Unknown command: "));
        Serial.println(command);
        Serial.println(F("Type 'help' for available commands."));
    }
}

// -----------------------------------------------------------------------------
// Console display functions
// -----------------------------------------------------------------------------

void DebugConsoleArduino::printHelp()
{
    Serial.println();
    Serial.println(F("----- Backpack Debug Commands -----"));
    Serial.println(F("help                       - Show this menu"));
    Serial.println(F("scan <UID>                 - Simulate a tag scan only"));
    Serial.println(F("add <UID>                  - Tell the core an item was added"));
    Serial.println(F("remove <UID>               - Tell the core an item was removed"));
    Serial.println(F("start <taskTypeNumber>     - Start a registered task"));
    Serial.println(F("stop                       - Stop the active task"));
    Serial.println(F("inventory                  - Print registered inventory"));
    Serial.println(F("status                     - Print current core status"));
    Serial.println(F("events                     - Print and drain queued events"));
    Serial.println(F("register <UID> <type>      - Register an RFID/type mapping"));
    Serial.println();
    Serial.println(F("UIDs must be hexadecimal: 4, 7, or 10 bytes."));
    Serial.println(F("Examples: 4A123C4D, 04A123C4D00102"));
    Serial.println(F("TaskType and ItemType numbers follow the enums in Task.h / Item.h."));
    Serial.println(F("-----------------------------------"));
    Serial.println();
}

void DebugConsoleArduino::printInventory()
{
    Serial.println();
    Serial.println(F("===== INVENTORY ====="));

    const uint8_t count = inventory.getItemCount();

    for (uint8_t i = 0; i < count; ++i)
    {
        const Item* item = inventory.getItemAt(i);

        if (item == nullptr)
        {
            continue;
        }

        Serial.print(F("Tag: "));
        printTagID(item->tagID);
        Serial.println();

        Serial.print(F("Item index: "));
        Serial.println(static_cast<unsigned int>(i));

        Serial.print(F("Type: "));
        Serial.println(itemName(item->type));

        Serial.print(F("Location: "));
        Serial.println(locationName(item->location));

        Serial.println();
    }

    Serial.print(F("Total items: "));
    Serial.println(static_cast<unsigned int>(count));
    Serial.println(F("====================="));
}

void DebugConsoleArduino::printStatus()
{
    Serial.println();
    Serial.println(F("===== BACKPACK STATUS ====="));

    // Numeric state output deliberately avoids assuming the exact enum
    // enumerator names. The value corresponds to the core's state enum.
    Serial.print(F("State value: "));
    Serial.println(stateName(backpack.getState()));

    Serial.print(F("Task active: "));
    Serial.println(backpack.isTaskActive() ? F("Yes") : F("No"));

    Serial.print(F("Task complete: "));
    Serial.println(backpack.isTaskComplete() ? F("Yes") : F("No"));

    Serial.print(F("Current task: "));
    Serial.println(taskName(backpack.getCurrentTask()));

    Serial.print(F("Next required item: "));
    Serial.println(itemName(backpack.getNextRequiredItem()));

    Serial.println(F("==========================="));
}

void DebugConsoleArduino::printEvents()
{
    Serial.println();
    Serial.println(F("===== QUEUED EVENTS (FIFO) ====="));

    uint8_t printed = 0;

    while (backpack.hasEvent())
    {
        const BackpackEvent event = backpack.getEvent();

        Serial.print(F("Event type value: "));
        Serial.println(eventName(event.type));

        Serial.print(F("Item: "));
        Serial.println(itemName(event.item));

        Serial.print(F("Task type value: "));
        Serial.println(taskName(event.task));

        Serial.println();
        ++printed;
    }

    if (printed == 0)
    {
        Serial.println(F("No queued events."));
    }

    Serial.println(F("================================"));
}

// -----------------------------------------------------------------------------
// Display helpers
// -----------------------------------------------------------------------------

const char* DebugConsoleArduino::itemName(ItemType type)
{
    switch (type)
    {
    case ItemType::Unknown:     return "Unknown";
    case ItemType::Wallet:      return "Wallet";
    case ItemType::Keys:        return "Keys";
    case ItemType::Phone:       return "Phone";
    case ItemType::Laptop:      return "Laptop";
    case ItemType::Charger:     return "Charger";
    case ItemType::WaterBottle: return "WaterBottle";
    }

    return "Invalid item type";
}

const char* DebugConsoleArduino::locationName(Location location)
{
    switch (location)
    {
    case Location::Unknown:  return "Unknown";
    case Location::Outside:  return "Outside";
    case Location::Backpack: return "Backpack";
    }

    return "Invalid location";
}

const char* DebugConsoleArduino::eventName(EventType type)
{
    switch (type)
    {
    case EventType::Unknown:            return "Unknown";
    case EventType::TaskStarted:        return "TaskStarted";
    case EventType::TaskStopped:        return "TaskStopped";
    case EventType::TaskComplete:       return "TaskComplete";
    case EventType::ItemRequired:       return "ItemRequired";
    case EventType::ItemAcquired:       return "ItemAcquired";
    case EventType::ItemAlreadyPresent: return "ItemAlreadyPresent";
    case EventType::ItemNotRequired:    return "ItemNotRequired";
    case EventType::UnknownRFID:        return "UnknownRFID";
    case EventType::ItemAdded:          return "ItemAdded";
    case EventType::ItemRemoved:        return "ItemRemoved";
    case EventType::TaskReminder:       return "TaskReminder";
    case EventType::InventoryFull:      return "InventoryFull";
    }

    return "Invalid event type";
}

const char* DebugConsoleArduino::stateName(BackpackState state)
{
    switch (state)
    {
    case BackpackState::IDLE:                return "Idle";
    case BackpackState::ACQUIRING_ITEMS:     return "Acquiring Items";
    case BackpackState::CHECKING_INVENTORY:  return "Checking Inventory";
    case BackpackState::TASK_REMINDER:       return "Task Reminder";
    }

    return "Invalid state";
}

const char* DebugConsoleArduino::taskName(TaskType type)
{
    switch (type)
    {
    case TaskType::None:         return "None";
    case TaskType::University:   return "University";
    case TaskType::Gym:          return "Gym";
    case TaskType::Work:         return "Work";
    }

    return "Invalid task type";
}

bool DebugConsoleArduino::parseUnsignedLong(const char* text,
                                            unsigned long& value)
{
    if (text == nullptr || *text == '\0' || *text == '-')
    {
        return false;
    }

    char* end = nullptr;
    const unsigned long parsed = strtoul(text, &end, 0);

    if (end == text || *end != '\0')
    {
        return false;
    }

    value = parsed;
    return true;
}


bool DebugConsoleArduino::parseTagID(
    const char* text,
    RFIDTagID& tagID)
{
    if (text == nullptr)
    {
        return false;
    }

    // Count the hexadecimal characters.
    uint8_t charCount = 0;

    while (text[charCount] != '\0')
    {
        // Each UID byte requires exactly two hexadecimal characters.
        if (charCount >= MAX_RFID_UID_BYTES * 2)
        {
            return false;
        }

        ++charCount;
    }

    // RC522 UIDs are commonly 4, 7, or 10 bytes long.
    if (charCount != 8 &&
        charCount != 14 &&
        charCount != 20)
    {
        return false;
    }

    tagID.length = charCount / 2;

    for (uint8_t i = 0; i < tagID.length; ++i)
    {
        const char highChar = text[i * 2];
        const char lowChar = text[i * 2 + 1];

        uint8_t highNibble;
        uint8_t lowNibble;

        // Convert the first hexadecimal character.
        if (highChar >= '0' && highChar <= '9')
        {
            highNibble = highChar - '0';
        }
        else if (highChar >= 'A' && highChar <= 'F')
        {
            highNibble = highChar - 'A' + 10;
        }
        else if (highChar >= 'a' && highChar <= 'f')
        {
            highNibble = highChar - 'a' + 10;
        }
        else
        {
            return false;
        }

        // Convert the second hexadecimal character.
        if (lowChar >= '0' && lowChar <= '9')
        {
            lowNibble = lowChar - '0';
        }
        else if (lowChar >= 'A' && lowChar <= 'F')
        {
            lowNibble = lowChar - 'A' + 10;
        }
        else if (lowChar >= 'a' && lowChar <= 'f')
        {
            lowNibble = lowChar - 'a' + 10;
        }
        else
        {
            return false;
        }

        // Combine the two 4-bit values into one byte.
        tagID.bytes[i] = (highNibble << 4) | lowNibble;
    }

    return true;
}


void DebugConsoleArduino::printTagID(
    const RFIDTagID& tagID)
{
    for (uint8_t i = 0; i < tagID.length; ++i)
    {
        // Arduino's HEX output omits leading zeros,
        // so print a zero explicitly when needed.
        if (tagID.bytes[i] < 0x10)
        {
            Serial.print('0');
        }

        Serial.print(tagID.bytes[i], HEX);
    }
}

