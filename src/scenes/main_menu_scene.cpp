#include "scene.h"
#include "app_context.h"
#include "ui.h"

#define THIS_SCENE MainMenuScene

void THIS_SCENE::Enter(AppContext& ctx) {
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2);
}

void THIS_SCENE::Leave(AppContext& ctx) {
}

void THIS_SCENE::Update(AppContext& ctx, float dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.running = false;
    }
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};

    ui.Label({800, 200}, "M.I.C.E", FontSize::Large);
    static bool toggle = false;
    toggle ^= ui.Button({800, 400, 320, 150}, "Click me", FontSize::Medium);
    Color c = toggle ? Colors::black : Colors::white;
    ctx.renderer.scene->DrawRectangle(900, 600, 120, 120, c);
    UI::DrawCursors(ctx);
}
