#include <app_context.h>
#include "scene.h"

void BasicScene::Update(AppContext& ctx, float dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.running = false;
    }
}

void BasicScene::Draw(AppContext& ctx) {
    MousePosition const& mps0 = ctx.mice.positions[0];
    MousePosition const& mps1 = ctx.mice.positions[1];

    ctx.renderer.scene->DrawRectangle(mps0.x, mps0.y, 25, 25, {255, 0, 0});

    ctx.renderer.scene->DrawRectangle(mps1.x, mps1.y, 25, 25, {0, 255, 0});
}