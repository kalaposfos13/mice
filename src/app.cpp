#include "app.h"
#include "ui.h"

#include <map>
#include <utility>

void App::Run() {
    current_scene = std::make_unique<MainMenuScene>();
    current_scene->Enter(ctx);

    ctx.running = true;
    auto dt_prev = std::chrono::steady_clock::now();

    while (ctx.running) {
        ctx.pad.Update();
        ctx.mice.UpdateState();

        auto now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration<double>(now - dt_prev).count();
        dt_prev = now;
        current_scene->Update(ctx, dt);

        ctx.renderer.BeginFrame();
        current_scene->Draw(ctx);
        ctx.renderer.EndFrame();
        if (ctx.next_scene) {
            current_scene->Leave(ctx);
            current_scene = std::move(ctx.next_scene);
            current_scene->Enter(ctx);
        }
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
