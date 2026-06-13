#include "app.h"
#include "ui.h"

#include <map>
#include <utility>

void App::Run() {
    ctx.scenes.Push<SettingsScene>();
    ctx.scenes.Update(0.0f);

    ctx.mice.Recenter(0);
    ctx.mice.Recenter(1);

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
    OrbisUserServiceInitializeParams param;
    param.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
    sceUserServiceInitialize(&param);
    sceUserServiceGetInitialUser(&ctx.user_id);
    LOG_INFO("userid: {:x}", (u32)ctx.user_id);
    ctx.pad.Init(ctx.user_id);
    ctx.mice.Init(ctx.user_id);
    UI::InitFonts(ctx);
}

App::~App() {
    LOG_INFO("App stopped.");
    sceSystemServiceLoadExec("EXIT", nullptr);
}
