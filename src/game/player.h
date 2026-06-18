#pragma once

#include "container.h"
#include "maths.h"

class Arm {
    Vec2 position;
    bool gripping{};
};

class Player {
    Arm arms[2];
    Container* held_container;
};