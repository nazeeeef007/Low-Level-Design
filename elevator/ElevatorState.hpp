#pragma once

#include "Direction.hpp"

class Elevator;
struct Request;

class ElevatorState {
public:
    virtual ~ElevatorState() = default;
    virtual void addRequest(Elevator& elevator, const Request& request) const = 0;
    [[nodiscard]] virtual Direction getDirection() const noexcept = 0;
    virtual void move(Elevator& elevator) const = 0;
};

class IdleState final : public ElevatorState {
public:
    void addRequest(Elevator& elevator, const Request& request) const override;
    [[nodiscard]] Direction getDirection() const noexcept override;
    void move(Elevator& elevator) const override;
};

class MovingUpState final : public ElevatorState {
public:
    void addRequest(Elevator& elevator, const Request& request) const override;
    [[nodiscard]] Direction getDirection() const noexcept override;
    void move(Elevator& elevator) const override;
};

class MovingDownState final : public ElevatorState {
public:
    void addRequest(Elevator& elevator, const Request& request) const override;
    [[nodiscard]] Direction getDirection() const noexcept override;
    void move(Elevator& elevator) const override;
};
