#include "app.h"

#include <map>
#include <utility>

void App::Run() {
    current_scene = std::make_unique<BasicScene>();
    current_scene->Enter(ctx);

    ctx.running = true;

    auto dt_next = std::chrono::steady_clock::now();
    auto dt_prev = dt_next;

    while (ctx.running) {
        ctx.pad.Update();
        ctx.mice.UpdateState();


        current_scene->Update(ctx, ((dt_next - dt_prev).count() / 1'000'000.0));
        dt_prev = dt_next;
        dt_next = std::chrono::steady_clock::now();

        ctx.renderer.BeginFrame();
        current_scene->Draw(ctx);
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
}

App::~App() {
    LOG_INFO("App stopped.");
    sceSystemServiceLoadExec("EXIT", nullptr);
}
