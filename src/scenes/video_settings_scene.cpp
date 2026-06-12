#include "scene.h"
#include "app_context.h"
#include "ui.h"

#define THIS_SCENE VideoSettingsScene

void THIS_SCENE::Enter(AppContext&) {}

void THIS_SCENE::Leave(AppContext&) {}

void THIS_SCENE::Update(AppContext& ctx, float) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};

    ui.Label({650, 200}, "VIDEO SETTINGS", FontSize::Large);

    ui.Label({700, 350}, "1920x1080");
    ui.Label({700, 450}, "VSync: ON");
}