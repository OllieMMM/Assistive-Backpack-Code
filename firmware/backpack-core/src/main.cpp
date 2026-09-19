#include <iostream>

#include "Task.h"

int main()
{
    Task universityTask;

    universityTask.type = TaskType::University;

    universityTask.requiredItems[0] = ItemType::Wallet;
    universityTask.requiredItems[1] = ItemType::Keys;
    universityTask.requiredItems[2] = ItemType::Phone;
    universityTask.requiredItems[3] = ItemType::Laptop;
    universityTask.requiredItems[4] = ItemType::Charger;

    universityTask.itemCount = 5;

    std::cout << "University task created!" << std::endl;
    std::cout << "Required items: "
              << static_cast<int>(universityTask.itemCount) << std::endl;
    std::cout << "Item 2: " 
              << static_cast<int>(universityTask.requiredItems[1]) 
              << std::endl;
    return 0;
}