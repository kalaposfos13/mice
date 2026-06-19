#pragma once

#include <array>
#include "arm.h"
#include "container.h"
#include "maths.h"

class Player {
public:
    std::array<Arm, 2> arms;
    Container* held_container;
};