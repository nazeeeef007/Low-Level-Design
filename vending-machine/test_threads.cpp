#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "VendingMachine.hpp"

std::mutex printMutex; // Separate mutex just to keep console output clean

void customerRoutine(VendingMachine& vm, Drink drink, int qty, int cash, int customerID) {
    try {
        vm.executePurchase(drink, qty, cash);
        
        std::lock_guard<std::mutex> lock(printMutex);
        std::cout << "[Customer " << customerID << "] Purchase Success: " << drink.getName() << std::endl;
    } catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(printMutex);
        std::cout << "[Customer " << customerID << "] Purchase Failed: " << e.what() << std::endl;
    }
}

int main() {
    // 10 drinks max, 100 coins max
    VendingMachine vm(10, 100);
    Drink cola(1, 100, "Cola"); // $1.00
    
    // Only stock 3 Colas
    vm.restockDrink(cola, 3);
    vm.restockChange(Change(100), 50);

    std::cout << "--- Starting Concurrent Simulation (10 customers for 3 Colas) ---" << std::endl;

    std::vector<std::thread> threads;
    for (int i = 1; i <= 10; ++i) {
        // All customers try to buy at once
        threads.emplace_back(customerRoutine, std::ref(vm), cola, 1, 100, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "--- Simulation Finished ---" << std::endl;
    return 0;
}