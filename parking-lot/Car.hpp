#ifndef CAR_HPP
#define CAR_HPP 
#include "Vehicle.hpp"
#include <iostream>

class Car: public Vehicle{
    public:
        
    void drive() override {
        std::cout << "Car is driving!" << std::endl;
    }

    VehicleType getType() const override {
        return VehicleType::Car;
    }
};

#endif