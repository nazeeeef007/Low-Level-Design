#ifndef DRINK_HPP
#define DRINK_HPP
#include <string>

class Drink {
private:
    int id;
    int price; // Changed to int (cents)
    std::string name;
public:
    Drink(int id, int price, std::string name) : id(id), price(price), name(name) {}
    Drink() : id(0), price(0), name("") {}

    int getId() const { return id; }
    int getPrice() const { return price; }
    std::string getName() const { return name; }

    // Comparator for std::map
    struct compareByDrinkId {
        bool operator()(const Drink& a, const Drink& b) const {
            return a.getId() < b.getId();
        }
    };
};
#endif