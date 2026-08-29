#pragma once

#include <string_view>

enum class Direction { UP, DOWN, IDLE };

inline constexpr std::string_view toString(Direction direction) noexcept {
    switch (direction) {
    case Direction::UP: return "UP";
    case Direction::DOWN: return "DOWN";
    case Direction::IDLE: return "IDLE";
    }
    return "UNKNOWN";
}
