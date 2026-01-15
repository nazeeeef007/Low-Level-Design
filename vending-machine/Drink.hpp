#ifndef DRINK_HPP
#define DRINK_HPP

#include "Product.hpp"

class Drink : public Product {
private:
    int volume;

public:
    Drink(int id, int price, const std::string& name, int volume)
        : Product(id, price, name), volume(volume) {}

    Drink() : Product(0, 0.0, ""), volume(0) {}


    int getVolume() const { return volume; }

    // Comparator for std::map / std::set
    struct compareByDrinkId {
        bool operator()(const Drink& a, const Drink& b) const {
            return a.getId() < b.getId();
        }
    };
};

#endif
