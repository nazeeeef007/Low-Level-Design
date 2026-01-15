#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class Product {
protected:
    std::string name;
    int price;
    int id;

    // Protected constructor → cannot instantiate Product directly
    Product(int id, int price, const std::string& name)
        : name(name), price(price), id(id) {}

public:
    virtual ~Product() = default;

    std::string getName() const {return name;}
    int getPrice() const {return price;}
    int getId() const {return id;}

    struct compareByProductId {
        bool operator()(const Product& a, const Product& b) const {
            return a.getId() < b.getId();
        }
    };
};

#endif
