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
        unsigned long tagValue = 0;

        if (tagText == nullptr || !parseUnsignedLong(tagText, tagValue))
        {
            Serial.println(F("Usage: scan <tagID>, e.g. scan 0x1001"));
            return;
        }

        Serial.print(F("Simulating RFID scan: 0x"));
        Serial.println(tagValue, HEX);
        backpack.itemScanned(static_cast<uint32_t>(tagValue));
        Serial.println(F("Scan passed to Backpack core."));
    }
    else if (strcmp(command, "add") == 0)
    {
        char* tagText = strtok(nullptr, " \t");
        unsigned long tagValue = 0;

        if (tagText == nullptr || !parseUnsignedLong(tagText, tagValue))
        {
            Serial.println(F("Usage: add <tagID>, e.g. add 0x1001"));
            return;
        }

        // itemAdded() owns the core-side inventory/FSM update. We do not
        // assume a return type here; the inventory/status commands can be
        // used to inspect the result.
        backpack.itemAdded(static_cast<uint32_t>(tagValue));
        Serial.println(F("Add command sent to Backpack core. Use 'inventory' or 'status' to verify."));
    }
    else if (strcmp(command, "remove") == 0)
    {
        char* tagText = strtok(nullptr, " \t");
        unsigned long tagValue = 0;

        if (tagText == nullptr || !parseUnsignedLong(tagText, tagValue))
        {
            Serial.println(F("Usage: remove <tagID>, e.g. remove 0x1001"));
            return;
        }

        // As above, keep this compatible whether itemRemoved() returns a
        // status value or simply performs the operation.
        backpack.itemRemoved(static_cast<uint32_t>(tagValue));
        Serial.println(F("Remove command sent to Backpack core. Use 'inventory' or 'status' to verify."));
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
        unsigned long tagValue = 0;
        unsigned long typeValue = 0;

        if (tagText == nullptr || typeText == nullptr ||
            !parseUnsignedLong(tagText, tagValue) ||
            !parseUnsignedLong(typeText, typeValue))
        {
            Serial.println(F("Usage: register <tagID> <numeric ItemType value>"));
            Serial.println(F("Example: register 0x1007 1"));
            Serial.println(F("Check Item.h for ItemType enum values."));
            return;
        }

        const bool success = inventory.registerItem(
            static_cast<uint32_t>(tagValue),
            static_cast<ItemType>(typeValue));

        Serial.println(success ? F("RFID mapping registered.")
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
    Serial.println(F("scan <tagID>               - Simulate a tag scan only"));
    Serial.println(F("add <tagID>                - Tell the core an item was added"));
    Serial.println(F("remove <tagID>             - Tell the core an item was removed"));
    Serial.println(F("start <taskTypeNumber>     - Start a registered task"));
    Serial.println(F("stop                       - Stop the active task"));
    Serial.println(F("inventory                  - Print registered inventory"));
    Serial.println(F("status                     - Print current core status"));
    Serial.println(F("events                     - Print and drain queued events"));
    Serial.println(F("register <tagID> <type>    - Register an RFID/type mapping"));
    Serial.println();
    Serial.println(F("Tag IDs may be decimal or hexadecimal (e.g. 4097 or 0x1001)."));
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

        Serial.print(F("Tag: 0x"));
        Serial.println(static_cast<unsigned long>(item->tagID), HEX);

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
    Serial.println(static_cast<int>(backpack.getState()));

    Serial.print(F("Task active: "));
    Serial.println(backpack.isTaskActive() ? F("Yes") : F("No"));

    Serial.print(F("Task complete: "));
    Serial.println(backpack.isTaskComplete() ? F("Yes") : F("No"));

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
        Serial.println(static_cast<int>(event.type));

        Serial.print(F("Item: "));
        Serial.println(itemName(event.item));

        Serial.print(F("Task type value: "));
        Serial.println(static_cast<int>(event.task));

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
