#include "app_context.h"
#include "scene.h"

#define THIS_SCENE VideoSettingsScene

static bool first_draw = false;

void THIS_SCENE::Enter() {
    first_draw = true;
    LOG_INFO("called");
}

void THIS_SCENE::Leave() {}

void THIS_SCENE::Update(double dt) {}

void THIS_SCENE::Draw() {
    if (first_draw) {
        LOG_INFO("called");
        first_draw = false;
    }
    UI ui{};

    ui.Label({650, 200}, "VIDEO SETTINGS", FontSize::Large);

    ui.Label({700, 350}, "1920x1080");
    ui.Label({700, 450}, "VSync: ON");
}