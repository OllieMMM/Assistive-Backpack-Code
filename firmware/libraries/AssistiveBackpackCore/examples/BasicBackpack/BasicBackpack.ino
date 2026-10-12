#include <Backpack.h>

void setup()
{
    Serial.begin(9600);

    Serial.println("Assistive Backpack - Core Library Test");

    // One inventory shared by the backpack and task manager.
    Inventory inventory;
    Backpack backpack(inventory);

    backpack.begin();

    Serial.println("Backpack core begin() called.");
}

void loop()
{
    // Intentionally empty for this initial compatibility test.
}
