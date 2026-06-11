#pragma once

#include "input/mouse.h"
#include "input/pad.h"

#include "renderer.h"

struct Settings {

};

class AppContext {
public:
    s32 user_id{};
    bool running;

    Renderer renderer{};

    Pad pad{};
    Mice mice{};

    Settings settings{};
};