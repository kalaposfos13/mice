#pragma once

#include "maths.h"
#include "display/graphics.h"

class Arm {
public:
    Vec2 base, target;
    bool gripping{};
    Color color;

    double upper_length{450.0};
    double lower_length{450.0};

    void Draw(bool right_side);
};