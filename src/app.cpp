#include "app.h"
#include "ui/ui.h"

#include <map>
#include <utility>

void App::Run() {
    ctx.mice.Recenter(0);
    ctx.mice.Recenter(1);

    ctx.scenes.Push<SceneDesignerScene>();
    ctx.scenes.Update(0.0f);

    ctx.running = true;
    auto dt_prev = std::chrono::system_clock::now();

    while (ctx.running) {
        ctx.pad.Update();
        ctx.mice.Update();

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
}

App::~App() {
    LOG_INFO("App stopped.");
    sceSystemServiceLoadExec("EXIT", nullptr);
}
