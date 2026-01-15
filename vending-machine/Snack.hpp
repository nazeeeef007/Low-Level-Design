#ifndef SNACK_HPP
#define SNACK_HPP 
#include "Product.hpp"
#include <string>
#include <stdexcept>

class Snack: public Product {
    private:
        int weight;
    public:
        Snack(int id, int price, const std::string& name, int weight)
        : Product(id, price, name), weight(weight) {}

        Snack() : Product(0, 0.0, ""), weight(0) {} 

        int getWeight() {return weight;}

        void setWeight(int newWeight) {
            if (newWeight <= 0) throw std::invalid_argument("Weight should be positive!");
            weight = newWeight;
        }

        struct compareBySnackWeight {
            bool operator() (const Snack & a, const Snack & b) const {
                return a.weight < b.weight;
            }
        };

};


#endif