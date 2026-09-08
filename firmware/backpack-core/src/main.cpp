#include <iostream>

// Function initialization
int add(int a, int b);

int main(int argc, char**argv){
    // Gives number of input arguments to the main function
    std::cout << argc;

    //Pointer to pointer (start of list)
    // argv[0] = filepath
    // argv[1+] = user inputs
    // .\backpack.exe(1,'hello') returns pointer to 2nd user input 'hello' 
    std::cout << argv[2];

    // Prints message to console.
    std::cout << "Hello, from AssistiveBackpackCode!\n";

    // Prints numbers 0-9 to the console on new lines.
    for (int i = 0; i < 10;  i++){
        std::cout << i << "\n";
    }

    // User variable definitions
    int x = 40;
    int y = -3;

    // Uses a basic add function to add x, y.
    int ans = add(x, y);

    std::cout << "\n" << ans;

}

// Function declaration 
int add(int a, int b){
    return a + b;
}


