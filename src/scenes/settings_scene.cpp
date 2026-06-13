#include "scene.h"
#include "app_context.h"
#include "ui.h"

#define THIS_SCENE SettingsScene

void THIS_SCENE::Enter(AppContext&) {}

void THIS_SCENE::Leave(AppContext&) {}

void THIS_SCENE::Update(AppContext& ctx, float) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};

    ui.Label({700, 150}, "SETTINGS", FontSize::Large);

    if (ui.Button({700, 350, 500, 80}, "VIDEO", FontSize::Medium)) {
        LOG_INFO("-> video");
        ctx.scenes.Push<VideoSettingsScene>();
    }

    // if (ui.Button({700, 450, 500, 80}, "AUDIO", FontSize::Medium)) {
    //     ctx.scenes.Push<AudioSettingsScene>();
    // }

    if (ui.Button({700, 550, 500, 80}, "ABOUT", FontSize::Medium)) {
        LOG_INFO("-> about");
        ctx.scenes.Push<AboutOverlayScene>();
    }
}