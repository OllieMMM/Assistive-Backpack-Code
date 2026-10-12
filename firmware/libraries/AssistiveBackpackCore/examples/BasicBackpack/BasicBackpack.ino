// Assitive Backpack Code
// This file implaments a custom inventory and event management system
// to work with the Assitive Backpack for our ESI 2026 project.
// More information https://github.com/OllieMMM/Assistive-Backpack-Code
// Written by Oliver McKinlay

#include <Backpack.h>

void setup()
{
    Serial.begin(9600);

    Serial.println("Assistive Backpack - Core Library Test");

    // One inventory shared by the backpack and task manager.
    Inventory inventory;
    Backpack backpack(inventory);

    backpack.begin();

    Serial.println("Backpack succesfully started");
}

void loop()
{
    // Intentionally empty for this initial compatibility test.
}
