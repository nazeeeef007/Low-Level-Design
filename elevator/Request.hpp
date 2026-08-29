#pragma once

#include "Direction.hpp"
#include "RequestSource.hpp"

struct Request {
    Direction direction{Direction::IDLE};
    RequestSource source{RequestSource::INTERNAL};
    int targetFloor{0};
};
