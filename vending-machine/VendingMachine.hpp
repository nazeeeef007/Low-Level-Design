#ifndef VENDINGMACHINE_HPP
#define VENDINGMACHINE_HPP

#include <mutex>
#include "Inventory.hpp"
#include "Drink.hpp"
#include "Change.hpp"

class VendingMachine {
private:
    Inventory inventory;
    mutable std::mutex machineMutex; // Mutex to synchronize access

public:
    VendingMachine() : inventory(0, 0) {} 

    VendingMachine(int maxDrinkQty, int maxChangeQty) 
        : inventory(maxDrinkQty, maxChangeQty) {}

    void restockDrink(Drink drink, int quantity) {
        std::lock_guard<std::mutex> lock(machineMutex); // Lock for the duration of this function
        inventory.restockDrink(drink, quantity);
    }

    void restockChange(Change change, int quantity) {
        std::lock_guard<std::mutex> lock(machineMutex);
        inventory.restockChange(change, quantity);
    }

    // Updated to take 'int' for consistency with internal logic
    void executePurchase(Drink drink, int quantity, int amountPaid) {
        std::lock_guard<std::mutex> lock(machineMutex);
        inventory.executePurchase(drink, quantity, amountPaid);
    }
};

#endif