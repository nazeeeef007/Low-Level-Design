#include "ElevatorSelectionStrategy.hpp"

#include <cstdlib>
#include <limits>
#include <tuple>

std::shared_ptr<Elevator> NearestElevatorStrategy::selectElevator(
    const std::vector<std::shared_ptr<Elevator>>& elevators,
    const Request& request) const {
    std::shared_ptr<Elevator> best;
    auto bestScore = std::tuple{std::numeric_limits<int>::max(),
                                std::numeric_limits<int>::max(),
                                std::numeric_limits<std::size_t>::max(),
                                std::numeric_limits<int>::max()};

    for (const auto& elevator : elevators) {
        const auto snapshot = elevator->snapshot();
        if (!snapshot.running || snapshot.load >= snapshot.capacity) continue;
        // Compatible direction wins, then distance, queue length, and stable id.
        const auto score = std::tuple{isSuitable(snapshot, request) ? 0 : 1,
                                      std::abs(snapshot.currentFloor - request.targetFloor),
                                      snapshot.pendingStops, snapshot.id};
        if (score < bestScore) {
            bestScore = score;
            best = elevator;
        }
    }
    return best;
}

bool NearestElevatorStrategy::isSuitable(const ElevatorSnapshot& elevator,
                                         const Request& request) noexcept {
    if (elevator.direction == Direction::IDLE) return true;
    if (request.direction == Direction::UP)
        return elevator.direction == Direction::UP && elevator.currentFloor <= request.targetFloor;
    if (request.direction == Direction::DOWN)
        return elevator.direction == Direction::DOWN && elevator.currentFloor >= request.targetFloor;
    return false;
}
