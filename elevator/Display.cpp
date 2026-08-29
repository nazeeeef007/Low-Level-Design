#include "Display.hpp"

#include <ostream>

Display::Display(std::ostream& output) : output_(output) {}

void Display::update(const ElevatorSnapshot& elevator) {
    std::lock_guard lock(mutex_);
    output_ << "Elevator " << elevator.id
            << " | floor " << elevator.currentFloor
            << " | " << toString(elevator.direction)
            << " | load " << elevator.load << '/' << elevator.capacity
            << " | stops " << elevator.pendingStops << '\n';
}
