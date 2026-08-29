#include "ElevatorSystem.hpp"

#include <stdexcept>

ElevatorSystem::ElevatorSystem(int elevatorCount, int minimumFloor, int maximumFloor,
                               int elevatorCapacity, std::chrono::milliseconds travelTime,
                               std::unique_ptr<ElevatorSelectionStrategy> strategy)
    : minimumFloor_(minimumFloor), maximumFloor_(maximumFloor),
      selectionStrategy_(std::move(strategy)) {
    if (elevatorCount <= 0) throw std::invalid_argument("elevator count must be positive");
    if (minimumFloor > maximumFloor) throw std::invalid_argument("invalid floor range");
    if (!selectionStrategy_) throw std::invalid_argument("selection strategy cannot be null");
    elevators_.reserve(static_cast<std::size_t>(elevatorCount));
    for (int id = 0; id < elevatorCount; ++id)
        elevators_.push_back(std::make_shared<Elevator>(id, minimumFloor, maximumFloor,
                                                        elevatorCapacity, travelTime));
}

ElevatorSystem::~ElevatorSystem() { shutdown(); }

ElevatorSystem& ElevatorSystem::getInstance() {
    static ElevatorSystem instance(4, 0, 20, 10);
    return instance;
}

void ElevatorSystem::start() {
    std::lock_guard lock(mutex_);
    if (started_) return;
    for (const auto& elevator : elevators_) elevator->start();
    started_ = true;
}

void ElevatorSystem::shutdown() {
    std::vector<std::shared_ptr<Elevator>> elevators;
    {
        std::lock_guard lock(mutex_);
        if (!started_) return;
        started_ = false;
        elevators = elevators_;
    }
    for (const auto& elevator : elevators) elevator->stopElevator();
}

int ElevatorSystem::requestElevator(int floor, Direction direction) {
    validateFloor(floor);
    if (direction == Direction::IDLE)
        throw std::invalid_argument("an external request must specify UP or DOWN");
    if ((floor == maximumFloor_ && direction == Direction::UP) ||
        (floor == minimumFloor_ && direction == Direction::DOWN))
        throw std::invalid_argument("request direction points outside the building");

    std::shared_ptr<Elevator> selected;
    const Request request{direction, RequestSource::EXTERNAL, floor};
    {
        // Serializing selection plus assignment prevents concurrent callers from all
        // choosing the same previously-idle car.
        std::lock_guard lock(mutex_);
        if (!started_) throw std::logic_error("elevator system is not running");
        selected = selectionStrategy_->selectElevator(elevators_, request);
        if (!selected) throw std::runtime_error("no elevator currently has capacity");
        selected->addRequest(request);
    }
    return selected->getId();
}

void ElevatorSystem::selectFloor(int elevatorId, int floor) {
    validateFloor(floor);
    std::lock_guard lock(mutex_);
    if (!started_) throw std::logic_error("elevator system is not running");
    if (elevatorId < 0 || static_cast<std::size_t>(elevatorId) >= elevators_.size())
        throw std::out_of_range("unknown elevator id");
    const auto& elevator = elevators_[static_cast<std::size_t>(elevatorId)];
    const auto current = elevator->getCurrentFloor();
    const auto direction = floor > current ? Direction::UP
                         : floor < current ? Direction::DOWN : Direction::IDLE;
    elevator->addRequest({direction, RequestSource::INTERNAL, floor});
}

void ElevatorSystem::addObserver(const std::shared_ptr<ElevatorObserver>& observer) {
    for (const auto& elevator : elevators_) elevator->addObserver(observer);
}

std::shared_ptr<Elevator> ElevatorSystem::getElevator(int elevatorId) const {
    if (elevatorId < 0 || static_cast<std::size_t>(elevatorId) >= elevators_.size())
        throw std::out_of_range("unknown elevator id");
    return elevators_[static_cast<std::size_t>(elevatorId)];
}

std::vector<ElevatorSnapshot> ElevatorSystem::snapshots() const {
    std::vector<ElevatorSnapshot> result;
    result.reserve(elevators_.size());
    for (const auto& elevator : elevators_) result.push_back(elevator->snapshot());
    return result;
}

bool ElevatorSystem::waitUntilIdle(std::chrono::milliseconds timeout) const {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    for (const auto& elevator : elevators_) {
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline || !elevator->waitUntilIdle(
                std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now))) return false;
    }
    return true;
}

void ElevatorSystem::validateFloor(int floor) const {
    if (floor < minimumFloor_ || floor > maximumFloor_)
        throw std::out_of_range("floor is outside the building");
}
