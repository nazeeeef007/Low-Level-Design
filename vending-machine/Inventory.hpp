#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include <map>
#include <algorithm>
#include <stdexcept>
#include "Drink.hpp"
#include "Change.hpp"

class Inventory {
private:
    // Changed to std::map for easier custom object handling
    std::map<Drink, int, Drink::compareByDrinkId> drinkQty;
    std::map<Change, int, Change::compareByChangeAmount> changeQty;
    int maxDrinkQty;
    int maxChangeQty;
    long long totalChange; // Sum of all change in cents

public:
    Inventory(int maxDrinkQty, int maxChangeQty) 
        : maxDrinkQty(maxDrinkQty), maxChangeQty(maxChangeQty), totalChange(0) {}

    void restockDrink(Drink drink, int quantity) {
        int oldQty = drinkQty.count(drink) ? drinkQty[drink] : 0;
        drinkQty[drink] = std::min(maxDrinkQty, oldQty + quantity);
    }

    void restockChange(Change change, int quantity) {
        int oldQty = changeQty.count(change) ? changeQty[change] : 0;
        int newQty = std::min(maxChangeQty, oldQty + quantity);
        changeQty[change] = newQty;
        totalChange += (newQty - oldQty) * change.getAmount();
    }

    void executePurchase(Drink drink, int quantity, int amountPaid) {
        if (drinkQty.find(drink) == drinkQty.end() || drinkQty[drink] < quantity) {
            throw std::runtime_error("Insufficient quantity of drink!");
        }

        int cost = drink.getPrice() * quantity;
        int changeDue = amountPaid - cost;

        if (changeDue < 0) throw std::runtime_error("Insufficient amount paid!");
        if (changeDue > totalChange) throw std::runtime_error("Insufficient funds in machine!");

        // Greedy algorithm for change
        for (auto it = changeQty.rbegin(); it != changeQty.rend(); ++it) {
            if (changeDue <= 0) break;
            
            int coinVal = it->first.getAmount();
            int needed = changeDue / coinVal;
            int used = std::min(needed, it->second);

            it->second -= used;
            changeDue -= used * coinVal;
            totalChange -= used * coinVal;
        }

        if (changeDue > 0) {
            throw std::runtime_error("Cannot provide exact change with available denominations!");
        }

        drinkQty[drink] -= quantity;
    }
};
#endif