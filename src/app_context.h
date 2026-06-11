#pragma once

#include "input/mouse.h"
#include "input/pad.h"
#include "renderer.h"
#include "scenes/scene.h"

#include <memory>

struct Settings {};

class AppContext {
public:
    s32 user_id{};
    bool running;
    std::unique_ptr<Scene> next_scene;

    Renderer renderer{};

    Pad pad{};
    Mice mice{};

    Settings settings{};
};