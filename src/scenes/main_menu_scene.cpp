#include "app_context.h"
#include "scene.h"
#include "ui.h"

#define THIS_SCENE MainMenuScene

void THIS_SCENE::Enter(AppContext& ctx) {
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2);
}

void THIS_SCENE::Leave(AppContext& ctx) {}

void THIS_SCENE::Update(AppContext& ctx, float dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.running = false;
    }
    if ((ctx.mice.clicked_btns[0] & MouseButtons::Middle) != 0) {
        ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2);
    }
    if ((ctx.mice.clicked_btns[1] & MouseButtons::Middle) != 0) {
        ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2);
    }
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};

    ui.Label({830, 200}, "M.I.C.E.", FontSize::Large);
    bool start = true;
    start &= ui.Button({800, 400, 160, 80}, "", FontSize::Large, ButtonReport::Held);
    start &= ui.Button({960, 400, 160, 80}, "", FontSize::Large, ButtonReport::Held);
    ui.Label({810, 400}, "S T A R T", FontSize::Large);
    Color c = start ? Colors::black : Colors::white;
    ctx.renderer.scene->DrawRectangle(900, 600, 120, 120, c);
    UI::DrawCursors(ctx);
}
