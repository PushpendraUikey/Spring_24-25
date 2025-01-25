#include <iostream>
#include <thread>
#include <chrono>

int main() {
    while (true) {
        std::cout << "Hello Pushpendra" << std::endl;
        // Wait for 0.5 seconds (500 milliseconds)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
