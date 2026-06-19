#include "app_context.h"
#include "scene.h"

#define THIS_SCENE SettingsScene

void THIS_SCENE::Enter() {}

void THIS_SCENE::Leave() {}

void THIS_SCENE::Update(double dt) {}

void THIS_SCENE::Draw() {
    UI ui{};

    ui.Panel({600, 100, 700, 700});
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