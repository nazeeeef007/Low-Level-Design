#pragma once

#include "Elevator.hpp"

#include <memory>
#include <vector>

class ElevatorSelectionStrategy {
public:
    virtual ~ElevatorSelectionStrategy() = default;
    [[nodiscard]] virtual std::shared_ptr<Elevator> selectElevator(
        const std::vector<std::shared_ptr<Elevator>>& elevators,
        const Request& request) const = 0;
};

class NearestElevatorStrategy final : public ElevatorSelectionStrategy {
public:
    [[nodiscard]] std::shared_ptr<Elevator> selectElevator(
        const std::vector<std::shared_ptr<Elevator>>& elevators,
        const Request& request) const override;

private:
    [[nodiscard]] static bool isSuitable(const ElevatorSnapshot& elevator,
                                         const Request& request) noexcept;
};
