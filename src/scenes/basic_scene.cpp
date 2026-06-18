#include "app_context.h"
#include "common/logging.h"
#include "scene.h"

static double elapsed_time = 0.0;

void BasicScene::Enter() {
    LOG_INFO("Entered basic scene");
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2);
}

void BasicScene::Leave() {
    LOG_INFO("Exited basic scene");
}

void BasicScene::Update(double dt) {
    elapsed_time += dt;
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_TRIANGLE)) {
        ctx.scenes.Push<BasicScene>();
    }
}

void BasicScene::Draw() {
    MousePosition const& mps0 = ctx.mice[0].position;
    MousePosition const& mps1 = ctx.mice[1].position;

    UI ui{};
    ui.Label({100, 300}, fmt::format("seconds: {}", std::floor(elapsed_time)));
}
