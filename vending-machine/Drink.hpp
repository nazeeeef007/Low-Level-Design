#ifndef DRINK_HPP
#define DRINK_HPP

#include "Product.hpp"

class Drink : public Product {
private:
    int volume;

public:
    Drink(int id, double price, const std::string& name, int volume)
        : Product(id, price, name), volume(volume) {}

    Drink() : Product(0, 0.0, ""), volume(0) {}

    int getId() const override { return id; }
    double getPrice() const override { return price; }
    std::string getName() const override { return name; }

    int getVolume() const { return volume; }

    // Comparator for std::map / std::set
    struct compareByDrinkId {
        bool operator()(const Drink& a, const Drink& b) const {
            return a.getId() < b.getId();
        }
    };
};

#endif
