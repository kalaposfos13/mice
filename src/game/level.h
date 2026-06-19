#pragma once

#include <array>
#include <vector>
#include "arm.h"
#include "container.h"
#include "obstacle.h"

class Level {
public:
    std::vector<Container> containers;
    std::vector<Polygon> forbidden_areas;
    std::vector<Obstacle> obstacles;

    std::array<Arm, 2> starting_pose;
};