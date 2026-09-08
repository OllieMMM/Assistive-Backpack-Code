#include <iostream>

#include "Item.h"

int main()
{
    Item wallet;

    wallet.tagID = 0x1234;
    wallet.type = ItemType::Wallet;
    wallet.location = Location::Backpack;

    std::cout << "Item created successfully!" << std::endl;

    return 0;
}