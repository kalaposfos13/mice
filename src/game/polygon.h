#pragma once

#include <vector>
#include "maths.h"

struct Pose {
public:
    Vec2 position;
    double rotation;
    double scale;
};

class Polygon {
public:
    std::vector<Vec2> points;
};