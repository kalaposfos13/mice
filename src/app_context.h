#pragma once

#include "common/assert.h"
#include "common/logging.h"
#include "input/mouse.h"
#include "input/pad.h"
#include "display/renderer.h"
#include "scenes/scene_manager.h"

#include <memory>
#include <filesystem>

extern std::filesystem::path APP_ROOT, WORKDIR_ROOT;

struct Settings {};

class AppContext {
public:
    s32 user_id{};
    bool running = false;
    bool draw_mice = true;
    bool accepting_inputs = true;
    SceneManager scenes{*this};

    Renderer renderer{};

    Pad pad{};
    Mice mice{};

    Settings settings{};
};