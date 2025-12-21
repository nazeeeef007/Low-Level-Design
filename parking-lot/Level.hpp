#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <unordered_map>
#include "Vehicle.hpp"
#include <stdexcept>
#include <optional>
#include <sstream>
#include <memory>
#include <iostream>
class Level {
    public:

        Level(int capacity): capacity(capacity) {
            lots.clear();
        }

        Level() {
            lots.clear();
            capacity = 8;
        }

        int getCapacity() {
            return capacity;
        }

        std::optional<std::shared_ptr<Vehicle>> getVehicleAtLot(int id) {
            if (id > capacity || id < 0) {
                std::stringstream ss;
                ss << "Lot id must be between 0 and " << capacity;
                throw std::invalid_argument(ss.str());
            }

            auto it = lots.find(id);
            if (it == lots.end()) return std::nullopt;

            return it->second;
        }

        void park(std::shared_ptr<Vehicle> vehicle, int id) {
            if (id < 0 || id >= capacity) {
                std::stringstream ss;
                ss << "Lot id must be between 0 and " << capacity;
                throw std::invalid_argument(ss.str());
            }

            if (lots.size() == capacity) {
                throw std::runtime_error("Level is full");
            }

            if (lots.find(id) != lots.end()) {
                throw std::runtime_error("Lot is already occupied");
            }

            lots[id] = std::move(vehicle);
        }


        void leave(int id) {
            if (lots.empty() || lots.find(id) == lots.end()) return;
            lots.erase(id);
        }




    private:
        int capacity;
        std::unordered_map<int, std::shared_ptr<Vehicle>> lots;

};

#endif