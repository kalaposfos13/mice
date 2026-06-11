#pragma once

#include "common/assert.h"
#include "common/logging.h"
#include "common/types.h"

#include <orbis/Sysmodule.h>
#include <orbis/SystemService.h>
#include <orbis/UserService.h>
#include <orbis/libkernel.h>

#include <cstdlib>
#include <memory>

#include "app_context.h"
#include "scenes/scene.h"

class App {
public:
    App();
    ~App();

    void Run();

    bool use_font = false;

    AppContext ctx{};
    std::unique_ptr<Scene> current_scene;
};
