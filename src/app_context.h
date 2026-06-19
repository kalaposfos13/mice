#pragma once

#include "common/assert.h"
#include "common/logging.h"
#include "display/renderer.h"
#include "input/input_manager.h"
#include "scenes/scene_manager.h"

#include <filesystem>
#include <memory>

extern std::filesystem::path APP_ROOT, WORKDIR_ROOT;

struct AppSettings {};

struct AppState {
    bool running = false;
    bool draw_mice = true;
    bool accepting_inputs = true;
};

class AppContext {
public:
    SceneManager scenes{};

    Renderer renderer{};

    s32 user_id{};
    InputManager input{};

    AppSettings settings{};
    AppState state;

    AppContext();
};

extern AppContext ctx;