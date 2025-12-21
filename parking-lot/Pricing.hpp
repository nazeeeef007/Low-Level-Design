#ifndef PRICING_HPP
#define PRICING_HPP

enum class VehicleType {
    Motorcycle,
    Car,
    Truck
};

inline double getHourlyPrice(VehicleType type) {
    switch (type) {
        case VehicleType::Motorcycle: return 1.5;
        case VehicleType::Car:        return 3.0;
        case VehicleType::Truck:      return 5.0;
        default: return 0.0;
    }
}

#endif
