#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class Product {
protected:
    std::string name;
    double price;
    int id;

    // Protected constructor → cannot instantiate Product directly
    Product(int id, double price, const std::string& name)
        : name(name), price(price), id(id) {}

public:
    virtual ~Product() = default;

    virtual std::string getName() const = 0;
    virtual double getPrice() const = 0;
    virtual int getId() const = 0;
};

#endif
