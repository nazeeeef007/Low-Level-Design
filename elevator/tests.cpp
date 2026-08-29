#include "ElevatorSystem.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

void testMovementAndOrdering() {
    Elevator elevator(0, 0, 10, 4, 1ms);
    elevator.start();
    elevator.addRequest({Direction::UP, RequestSource::INTERNAL, 8});
    elevator.addRequest({Direction::UP, RequestSource::INTERNAL, 3});
    elevator.addRequest({Direction::DOWN, RequestSource::INTERNAL, 1});
    assert(elevator.waitUntilIdle(1s));
    // All three destinations are above the starting floor, so SCAN order is 1, 3, 8.
    assert(elevator.getCurrentFloor() == 8);
    elevator.stopElevator();
}

void testCapacity() {
    Elevator elevator(0, 0, 2, 3, 0ms);
    assert(elevator.boardPassengers(2));
    assert(!elevator.boardPassengers(2));
    assert(elevator.snapshot().load == 2);
    elevator.exitPassengers();
    assert(elevator.snapshot().load == 1);
}

void testValidation() {
    ElevatorSystem system(1, 0, 5, 4, 0ms);
    system.start();
    bool rejected = false;
    try { (void)system.requestElevator(6, Direction::UP); }
    catch (const std::out_of_range&) { rejected = true; }
    assert(rejected);
    system.shutdown();
}

void testConcurrentRequests() {
    ElevatorSystem system(4, 0, 20, 10, 1ms);
    system.start();
    std::vector<std::thread> callers;
    for (int i = 0; i < 24; ++i) {
        callers.emplace_back([&system, i] {
            const int floor = 1 + (i % 18);
            const Direction direction = floor < 10 ? Direction::UP : Direction::DOWN;
            const int id = system.requestElevator(floor, direction);
            system.selectFloor(id, direction == Direction::UP ? 19 : 0);
        });
    }
    for (auto& caller : callers) caller.join();
    assert(system.waitUntilIdle(3s));
    for (const auto& snapshot : system.snapshots()) {
        assert(snapshot.currentFloor >= 0 && snapshot.currentFloor <= 20);
        assert(snapshot.pendingStops == 0);
    }
    system.shutdown();
}

int main() {
    testMovementAndOrdering();
    testCapacity();
    testValidation();
    testConcurrentRequests();
    std::cout << "All elevator tests passed\n";
}
