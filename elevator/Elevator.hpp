#pragma once

#include "ElevatorObserver.hpp"
#include "ElevatorState.hpp"
#include "Request.hpp"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

class Elevator {
public:
    Elevator(int id, int minimumFloor, int maximumFloor, int capacity,
             std::chrono::milliseconds travelTime = std::chrono::milliseconds{100});
    ~Elevator();

    Elevator(const Elevator&) = delete;
    Elevator& operator=(const Elevator&) = delete;

    void start();
    void stopElevator();
    void addRequest(const Request& request);
    [[nodiscard]] bool boardPassengers(int count = 1);
    void exitPassengers(int count = 1);

    void addObserver(const std::shared_ptr<ElevatorObserver>& observer);
    [[nodiscard]] ElevatorSnapshot snapshot() const;
    [[nodiscard]] bool waitUntilFloor(int floor, std::chrono::milliseconds timeout) const;
    [[nodiscard]] bool waitUntilIdle(std::chrono::milliseconds timeout) const;

    [[nodiscard]] int getId() const noexcept { return id_; }
    [[nodiscard]] int getCurrentFloor() const { return snapshot().currentFloor; }
    [[nodiscard]] Direction getDirection() const { return snapshot().direction; }
    [[nodiscard]] bool isRunning() const { return snapshot().running; }

private:
    friend class IdleState;
    friend class MovingUpState;
    friend class MovingDownState;

    void run();
    void moveIdle();
    void moveUp();
    void moveDown();
    void enqueueRequest(const Request& request);
    void chooseDirectionUnlocked();
    void setStateUnlocked(Direction direction);
    void notifyObservers();
    [[nodiscard]] bool hasRequestsUnlocked() const noexcept;
    [[nodiscard]] std::size_t pendingStopsUnlocked() const noexcept;

    const int id_;
    const int minimumFloor_;
    const int maximumFloor_;
    const int capacity_;
    const std::chrono::milliseconds travelTime_;

    mutable std::mutex mutex_;
    mutable std::condition_variable condition_;
    std::thread worker_;
    int currentFloor_{0};
    int load_{0};
    bool running_{false};
    bool stopRequested_{false};
    std::set<int> upRequests_;
    std::set<int, std::greater<int>> downRequests_;
    std::unique_ptr<ElevatorState> state_;
    std::vector<std::shared_ptr<ElevatorObserver>> observers_;
};
