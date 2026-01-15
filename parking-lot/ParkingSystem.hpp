#ifndef PARKINGSYSTEM_HPP
#define PARKINGSYSTEM_HPP
#include "Access.hpp"
#include "Vehicle.hpp"
#include "Level.hpp"
#include "Entry.hpp"
#include "Exit.hpp"
#include <vector>
#include <unordered_set>
#include <random>

class ParkingSystem {
    public:
        ParkingSystem(int numLevels, int numEntries, int numExits, unsigned seed = std::random_device{}())
            : numLevels(numLevels),
            numEntries(numEntries),
            numExits(numExits),
            gen(seed)
        {
            levels.assign(numLevels, Level());
            entries.assign(numEntries, Entry());
            exits.assign(numExits, Exit());
        }

        void enqueEntry(std::shared_ptr<Vehicle> vehicle) {
            if (numEntries == 0) return;
            std::uniform_int_distribution<int> dist(0, numEntries - 1);
            int pos = dist(gen);
            entries[pos].push(vehicle);
        }

        void enqueExit(std::shared_ptr<Vehicle> vehicle) {
            if (numEntries == 0) return;
            std::uniform_int_distribution<int> dist(0, numExits - 1);
            int pos = dist(gen);
            exits[pos].push(vehicle);
        }

        std::optional<std::shared_ptr<Vehicle>> dequeEntry() {
            std::uniform_int_distribution<int> dist(0, numEntries - 1);
            int pos = dist(gen);
            if (entries[pos].empty()) return std::nullopt;

            std::pair<std::optional<std::shared_ptr<Vehicle>>  ,double> v = entries[pos].pop();

            if (v.first.has_value() && !vehicleMap.count(v.first.value())) {
                vehicleMap[v.first] = v.second;
                return v.first;
            }

        }

        void park(std::shared_ptr<Vehicle> vehicle) {
            bool found = 0;
            for (int levelIndex = 0; levelIndex < numLevels; levelIndex++) {
                Level curLevel = levels[levelIndex];
                int levelCapacity = curLevel.getCapacity();
                for (int lot = 0; lot < levelCapacity; lot++) {
                    if (curLevel.park(vehicle, lot)) {
                        found = 1;
                        break;
                    }
                }
                if (found) break;
            }

            if (!found) {
                
            }

        }



    private:
        int numLevels, numEntries, numExits;
        std::vector<Level> levels;
        std::unordered_map<std::shared_ptr<Vehicle>, double> vehicleMap;
        std::vector<Entry> entries;
        std::vector<Exit> exits;
        std::mt19937 gen;

};

#endif