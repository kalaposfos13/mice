#pragma once

#include <vector>
#include "maths.h"

struct Pose {
    Vec2 position;
    double rotation;
    double scale;
};

class Polygon {
    std::vector<Vec2> points;
};