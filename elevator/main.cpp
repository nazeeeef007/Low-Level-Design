#include "Display.hpp"
#include "ElevatorSystem.hpp"

#include <chrono>
#include <iostream>
#include <memory>

int main() {
    using namespace std::chrono_literals;

    ElevatorSystem system(3, 0, 12, 8, 150ms);
    system.addObserver(std::make_shared<Display>(std::cout));
    system.start();

    const int first = system.requestElevator(4, Direction::UP);
    const int second = system.requestElevator(9, Direction::DOWN);

    // Destination buttons are pressed after each passenger's car arrives.
    if (!system.getElevator(first)->waitUntilFloor(4, 5s) ||
        !system.getElevator(second)->waitUntilFloor(9, 5s)) {
        std::cerr << "Timed out waiting for pickup\n";
        return 1;
    }
    system.selectFloor(first, 11);
    system.selectFloor(second, 2);

    if (!system.waitUntilIdle(10s)) {
        std::cerr << "Timed out waiting for elevators\n";
        return 1;
    }
    system.shutdown();
    return 0;
}
