#pragma once

#include "ElevatorObserver.hpp"

#include <iosfwd>
#include <mutex>

class Display final : public ElevatorObserver {
public:
    explicit Display(std::ostream& output);
    void update(const ElevatorSnapshot& elevator) override;

private:
    std::ostream& output_;
    std::mutex mutex_;
};
