#include "app_context.h"
#include "common/logging.h"
#include "scene.h"
#include "ui/ui.h"

static double elapsed_time = 0.0;

void BasicScene::Enter(AppContext& ctx) {
    LOG_INFO("Entered basic scene");
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2);
}

void BasicScene::Leave(AppContext& ctx) {
    LOG_INFO("Exited basic scene");
}

void BasicScene::Update(AppContext& ctx, double dt) {
    elapsed_time += dt;
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.running = false;
    }
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_TRIANGLE)) {
        ctx.scenes.Push<BasicScene>();
    }
}

void BasicScene::Draw(AppContext& ctx) {
    MousePosition const& mps0 = ctx.mice[0].position;
    MousePosition const& mps1 = ctx.mice[1].position;

    UI ui{ctx};
    ui.Label({100, 300}, fmt::format("seconds: {}", std::floor(elapsed_time)));
}
