#include "scene.h"
#include "app_context.h"
#include "common/logging.h"

void BasicScene::Enter(AppContext& ctx) {
    LOG_INFO("Entered basic scene");
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2);
}

void BasicScene::Leave(AppContext& ctx) {
    LOG_INFO("Exited basic scene");
}

void BasicScene::Update(AppContext& ctx, float dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.running = false;
    }
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_TRIANGLE)) {
        ctx.next_scene = std::make_unique<BasicScene>();
    }
}

void BasicScene::Draw(AppContext& ctx) {
    MousePosition const& mps0 = ctx.mice.positions[0];
    MousePosition const& mps1 = ctx.mice.positions[1];

    ctx.renderer.scene->DrawRectangle(mps0.x, mps0.y, 25, 25, {255, 0, 0});

    ctx.renderer.scene->DrawRectangle(mps1.x, mps1.y, 25, 25, {0, 255, 0});
}
