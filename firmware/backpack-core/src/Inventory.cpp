#include <Inventory.h>
#include <Item.h>

Inventory inv;

int main_inventory()
{
    // Does not work (build with CMake) because the function shave not yet been implemented. 
    inv.addItem(1234);
    return 0;
}