#pragma once

#include <vector>
#include "container.h"
#include "obstacle.h"

class Level {
    std::vector<Container> containers;
    std::vector<Polygon> forbidden_areas;
    std::vector<Obstacle> obstacles;

    Pose starting_pose;
};