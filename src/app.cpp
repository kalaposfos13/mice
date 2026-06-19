#include "app.h"
#include "display/ui/ui.h"
#include "scenes/game_scene.h"

#include <map>
#include <utility>

void App::Run() {
    ctx.state.running = true;
    auto dt_prev = std::chrono::system_clock::now();

    while (ctx.state.running) {
        ctx.input.Update();

        auto now = std::chrono::system_clock::now();
        double dt = std::chrono::duration<double>(now - dt_prev).count();
        dt_prev = now;
        ctx.scenes.Update(dt);

        ctx.renderer.BeginFrame();
        ctx.scenes.Draw();
        ctx.renderer.EndFrame();
    }
}

App::App() {
    UI::InitFonts();

    ctx.scenes.Push<GameScene>(std::move(SaveData{}));
    ctx.scenes.Update(0.0f);
}

App::~App() {
    LOG_INFO("App stopped.");
    sceSystemServiceLoadExec("EXIT", nullptr);
}
