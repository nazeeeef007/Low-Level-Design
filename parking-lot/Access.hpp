#ifndef ACCESS_HPP
#define ACCESS_HPP
#include <queue>
#include "Vehicle.hpp"
#include <memory>
#include <optional>
#include <random>
#include<chrono>

class Access {
    public: 
        ~Access() = default;
        void push(std::shared_ptr<Vehicle> vehicle) {
            vehicleQueue.push(vehicle);
        }

        std::pair<std::optional<std::shared_ptr<Vehicle>>, double> pop() {
            if (vehicleQueue.empty()) {
                return {std::nullopt, 0.0};
            }

            auto vehicle = vehicleQueue.front();
            vehicleQueue.pop();

            // current time in seconds
            double entryTime =
                std::chrono::duration<double>(
                    std::chrono::steady_clock::now().time_since_epoch()
                ).count();

            return {vehicle, entryTime};
        }

        bool empty() {
            return vehicleQueue.empty();
        }
    protected:
        std::queue<std::shared_ptr<Vehicle>> vehicleQueue;
};

#endif