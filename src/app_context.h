#pragma once

#include "common/assert.h"
#include "common/logging.h"
#include "display/renderer.h"
#include "input/mouse.h"
#include "input/pad.h"
#include "scenes/scene_manager.h"

#include <filesystem>
#include <memory>

extern std::filesystem::path APP_ROOT, WORKDIR_ROOT;

struct Settings {};

class AppContext {
public:
    s32 user_id{};
    bool running = false;
    bool draw_mice = true;
    bool accepting_inputs = true;
    SceneManager scenes{};

    Renderer renderer{};

    Pad pad{};
    Mice mice{};

    Settings settings{};

    AppContext();
};

extern AppContext ctx;