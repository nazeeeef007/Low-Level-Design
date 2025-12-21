#ifndef MOTORCYCLE_HPP
#define MOTORCYCLE_HPP 
#include "Vehicle.hpp"
#include <iostream>

class Motorcycle : public Vehicle {
    public: 
        void drive() override {
            std::cout << "Motorcycle is riding!" << std::endl;
        }

        VehicleType getType() const override {
            return VehicleType::Motorcycle;
        }
};

#endif