#ifndef VEHICLE_HPP
#define VEHICLE_HPP
#include <string>
#include "Pricing.hpp"
#include <memory>

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual void drive() = 0;
    virtual VehicleType getType() const = 0;

    const std::string& getPlateNumber() const {
        return plateNumber;
    }

protected:
    Vehicle(std::string plate) : plateNumber(std::move(plate)) {}

private:
    std::string plateNumber;
};


struct VehicleEqualityComparator{
    bool operator()(const std::shared_ptr<Vehicle> & a, const std::shared_ptr<Vehicle> & b) const {
        return a->getPlateNumber() != b->getPlateNumber();
    }
};


#endif