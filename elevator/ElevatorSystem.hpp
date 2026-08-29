#pragma once

#include "ElevatorSelectionStrategy.hpp"

#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

class ElevatorSystem {
public:
    ElevatorSystem(int elevatorCount, int minimumFloor, int maximumFloor,
                   int elevatorCapacity,
                   std::chrono::milliseconds travelTime = std::chrono::milliseconds{100},
                   std::unique_ptr<ElevatorSelectionStrategy> strategy =
                       std::make_unique<NearestElevatorStrategy>());
    ~ElevatorSystem();

    ElevatorSystem(const ElevatorSystem&) = delete;
    ElevatorSystem& operator=(const ElevatorSystem&) = delete;

    // A convenient default singleton, while the public constructor keeps tests configurable.
    static ElevatorSystem& getInstance();

    void start();
    void shutdown();
    [[nodiscard]] int requestElevator(int floor, Direction direction);
    void selectFloor(int elevatorId, int floor);
    void addObserver(const std::shared_ptr<ElevatorObserver>& observer);

    [[nodiscard]] std::shared_ptr<Elevator> getElevator(int elevatorId) const;
    [[nodiscard]] std::vector<ElevatorSnapshot> snapshots() const;
    [[nodiscard]] bool waitUntilIdle(std::chrono::milliseconds timeout) const;

private:
    void validateFloor(int floor) const;

    const int minimumFloor_;
    const int maximumFloor_;
    mutable std::mutex mutex_;
    bool started_{false};
    std::unique_ptr<ElevatorSelectionStrategy> selectionStrategy_;
    std::vector<std::shared_ptr<Elevator>> elevators_;
};
