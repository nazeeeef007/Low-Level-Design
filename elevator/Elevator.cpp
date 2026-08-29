#include "Elevator.hpp"

#include <algorithm>
#include <stdexcept>

Direction IdleState::getDirection() const noexcept { return Direction::IDLE; }
Direction MovingUpState::getDirection() const noexcept { return Direction::UP; }
Direction MovingDownState::getDirection() const noexcept { return Direction::DOWN; }
void IdleState::addRequest(Elevator& elevator, const Request& request) const {
    elevator.enqueueRequest(request);
}
void MovingUpState::addRequest(Elevator& elevator, const Request& request) const {
    elevator.enqueueRequest(request);
}
void MovingDownState::addRequest(Elevator& elevator, const Request& request) const {
    elevator.enqueueRequest(request);
}
void IdleState::move(Elevator& elevator) const { elevator.moveIdle(); }
void MovingUpState::move(Elevator& elevator) const { elevator.moveUp(); }
void MovingDownState::move(Elevator& elevator) const { elevator.moveDown(); }

Elevator::Elevator(int id, int minimumFloor, int maximumFloor, int capacity,
                   std::chrono::milliseconds travelTime)
    : id_(id), minimumFloor_(minimumFloor), maximumFloor_(maximumFloor),
      capacity_(capacity), travelTime_(travelTime), currentFloor_(minimumFloor),
      state_(std::make_unique<IdleState>()) {
    if (minimumFloor > maximumFloor) throw std::invalid_argument("minimum floor exceeds maximum floor");
    if (capacity <= 0) throw std::invalid_argument("elevator capacity must be positive");
    if (travelTime.count() < 0) throw std::invalid_argument("travel time cannot be negative");
}

Elevator::~Elevator() { stopElevator(); }

void Elevator::start() {
    std::lock_guard lock(mutex_);
    if (running_) return;
    stopRequested_ = false;
    running_ = true;
    worker_ = std::thread(&Elevator::run, this);
}

void Elevator::stopElevator() {
    {
        std::lock_guard lock(mutex_);
        if (!running_ && !worker_.joinable()) return;
        stopRequested_ = true;
        condition_.notify_all();
    }
    if (worker_.joinable()) worker_.join();
}

void Elevator::addRequest(const Request& request) {
    {
        std::lock_guard lock(mutex_);
        if (request.targetFloor < minimumFloor_ || request.targetFloor > maximumFloor_)
            throw std::out_of_range("requested floor is outside the building");
        state_->addRequest(*this, request);
        chooseDirectionUnlocked();
        condition_.notify_all();
    }
    notifyObservers();
}

bool Elevator::boardPassengers(int count) {
    if (count <= 0) throw std::invalid_argument("passenger count must be positive");
    {
        std::lock_guard lock(mutex_);
        if (load_ + count > capacity_) return false;
        load_ += count;
    }
    notifyObservers();
    return true;
}

void Elevator::exitPassengers(int count) {
    if (count <= 0) throw std::invalid_argument("passenger count must be positive");
    {
        std::lock_guard lock(mutex_);
        if (count > load_) throw std::logic_error("more passengers exited than were onboard");
        load_ -= count;
    }
    notifyObservers();
}

void Elevator::addObserver(const std::shared_ptr<ElevatorObserver>& observer) {
    if (!observer) throw std::invalid_argument("observer cannot be null");
    std::lock_guard lock(mutex_);
    observers_.push_back(observer);
}

ElevatorSnapshot Elevator::snapshot() const {
    std::lock_guard lock(mutex_);
    return {id_, currentFloor_, state_->getDirection(), load_, capacity_, running_,
            pendingStopsUnlocked()};
}

bool Elevator::waitUntilFloor(int floor, std::chrono::milliseconds timeout) const {
    if (floor < minimumFloor_ || floor > maximumFloor_)
        throw std::out_of_range("floor is outside the building");
    std::unique_lock lock(mutex_);
    return condition_.wait_for(lock, timeout, [this, floor] {
        return currentFloor_ == floor || !running_;
    }) && currentFloor_ == floor;
}

bool Elevator::waitUntilIdle(std::chrono::milliseconds timeout) const {
    std::unique_lock lock(mutex_);
    return condition_.wait_for(lock, timeout, [this] {
        return !hasRequestsUnlocked() && state_->getDirection() == Direction::IDLE;
    });
}

void Elevator::run() {
    for (;;) {
        {
            std::unique_lock lock(mutex_);
            condition_.wait(lock, [this] { return stopRequested_ || hasRequestsUnlocked(); });
            if (stopRequested_) break;
            chooseDirectionUnlocked();
            state_->move(*this);
        }
        notifyObservers();
        std::this_thread::sleep_for(travelTime_);
    }
    {
        std::lock_guard lock(mutex_);
        running_ = false;
        setStateUnlocked(Direction::IDLE);
        condition_.notify_all();
    }
    notifyObservers();
}

void Elevator::moveIdle() { chooseDirectionUnlocked(); }

void Elevator::enqueueRequest(const Request& request) {
    // Called by the active state while Elevator::mutex_ is held.
    if (request.targetFloor > currentFloor_) upRequests_.insert(request.targetFloor);
    else if (request.targetFloor < currentFloor_) downRequests_.insert(request.targetFloor);
}

void Elevator::moveUp() {
    if (upRequests_.empty()) {
        setStateUnlocked(downRequests_.empty() ? Direction::IDLE : Direction::DOWN);
        return;
    }
    ++currentFloor_;
    upRequests_.erase(currentFloor_);
    if (upRequests_.empty())
        setStateUnlocked(downRequests_.empty() ? Direction::IDLE : Direction::DOWN);
    condition_.notify_all();
}

void Elevator::moveDown() {
    if (downRequests_.empty()) {
        setStateUnlocked(upRequests_.empty() ? Direction::IDLE : Direction::UP);
        return;
    }
    --currentFloor_;
    downRequests_.erase(currentFloor_);
    if (downRequests_.empty())
        setStateUnlocked(upRequests_.empty() ? Direction::IDLE : Direction::UP);
    condition_.notify_all();
}

void Elevator::chooseDirectionUnlocked() {
    const auto direction = state_->getDirection();
    if (direction == Direction::UP && !upRequests_.empty()) return;
    if (direction == Direction::DOWN && !downRequests_.empty()) return;
    if (direction == Direction::UP && !downRequests_.empty()) { setStateUnlocked(Direction::DOWN); return; }
    if (direction == Direction::DOWN && !upRequests_.empty()) { setStateUnlocked(Direction::UP); return; }
    if (!upRequests_.empty() && !downRequests_.empty()) {
        const int upDistance = *upRequests_.begin() - currentFloor_;
        const int downDistance = currentFloor_ - *downRequests_.begin();
        setStateUnlocked(upDistance <= downDistance ? Direction::UP : Direction::DOWN);
    } else if (!upRequests_.empty()) setStateUnlocked(Direction::UP);
    else if (!downRequests_.empty()) setStateUnlocked(Direction::DOWN);
    else setStateUnlocked(Direction::IDLE);
}

void Elevator::setStateUnlocked(Direction direction) {
    if (state_ && state_->getDirection() == direction) return;
    if (direction == Direction::UP) state_ = std::make_unique<MovingUpState>();
    else if (direction == Direction::DOWN) state_ = std::make_unique<MovingDownState>();
    else state_ = std::make_unique<IdleState>();
}

void Elevator::notifyObservers() {
    const auto value = snapshot();
    std::vector<std::shared_ptr<ElevatorObserver>> live;
    {
        std::lock_guard lock(mutex_);
        live = observers_;
    }
    for (const auto& observer : live) observer->update(value);
}

bool Elevator::hasRequestsUnlocked() const noexcept {
    return !upRequests_.empty() || !downRequests_.empty();
}

std::size_t Elevator::pendingStopsUnlocked() const noexcept {
    return upRequests_.size() + downRequests_.size();
}
