#ifndef TRUCK_HPP
#define TRUCK_HPP 
#include "Vehicle.hpp"
#include <iostream>

class Truck : public Vehicle{
    public:
        void drive() override {
            std::cout << "Truck is driving!" << std::endl;
        }

        VehicleType getType() const override {
            return VehicleType::Truck;
        }
};

#endif