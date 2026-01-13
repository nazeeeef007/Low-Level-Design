#include <iostream>
#include <vector>
#include <string>
#include "VendingMachine.hpp"

// Utility to format cents into a readable string (e.g., 150 -> $1.50)
std::string formatMoney(int cents) {
    return "$" + std::to_string(cents / 100) + "." + (cents % 100 < 10 ? "0" : "") + std::to_string(cents % 100);
}

int main() {
    // 1. Initialize Machine: Max 10 of each drink, Max 100 of each coin type
    VendingMachine vm(10, 100);

    // 2. Setup Inventory (Restocking)
    Drink cola(1, 150, "Cola");      // $1.50
    Drink water(2, 100, "Water");    // $1.00
    Drink juice(3, 225, "Juice");    // $2.25

    std::cout << "--- Initializing Machine ---" << std::endl;
    vm.restockDrink(cola, 5);
    vm.restockDrink(water, 10);
    
    // Load the machine with change
    vm.restockChange(Change(100), 10); // 10 x $1.00
    vm.restockChange(Change(25), 20);  // 20 x $0.25
    vm.restockChange(Change(10), 50);  // 50 x $0.10
    std::cout << "Machine restocked with drinks and change.\n" << std::endl;

    // 3. Customer Simulation
    struct CustomerAction {
        std::string name;
        Drink drink;
        int qty;
        int cash;
    };

    std::vector<CustomerAction> customers = {
        {"Alice", cola, 1, 200},   // Should succeed, $0.50 change
        {"Bob", water, 2, 500},    // Should succeed, $3.00 change
        {"Charlie", juice, 1, 300},// Should fail (Out of stock)
        {"Dave", cola, 1, 100},    // Should fail (Insufficient payment)
        {"Eve", cola, 10, 2000}    // Should fail (Insufficient quantity)
    };

    for (const auto& action : customers) {
        std::cout << "Customer " << action.name << " wants " << action.qty << " " << action.drink.getName() 
                  << " and pays " << formatMoney(action.cash) << std::endl;
        try {
            vm.executePurchase(action.drink, action.qty, action.cash);
            std::cout << ">>> Result: Success! Enjoy your " << action.drink.getName() << ".\n" << std::endl;
        } catch (const std::exception& e) {
            std::cout << ">>> Result: Transaction Failed - " << e.what() << "\n" << std::endl;
        }
    }

    return 0;
}