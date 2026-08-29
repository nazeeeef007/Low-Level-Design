#pragma once

#include "Direction.hpp"

#include <cstddef>

struct ElevatorSnapshot {
    int id;
    int currentFloor;
    Direction direction;
    int load;
    int capacity;
    bool running;
    std::size_t pendingStops;
};

class ElevatorObserver {
public:
    virtual ~ElevatorObserver() = default;
    virtual void update(const ElevatorSnapshot& elevator) = 0;
};
