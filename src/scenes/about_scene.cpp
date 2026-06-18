#include "app_context.h"
#include "scene.h"
#include "ui/ui.h"

#define THIS_SCENE AboutOverlayScene

void THIS_SCENE::Enter() {}

void THIS_SCENE::Leave() {}

void THIS_SCENE::Update(double dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
}

void THIS_SCENE::Draw() {

    UI ui{};

    ctx.renderer.scene->DrawRectangle(1120, 300, 550, 350, Colors::blue);

    ui.Label({700 + 600, 350}, "M.I.C.E.");
    ui.Label({600 + 600, 450}, "Overlay test scene");
    ui.Label({550 + 600, 550}, "Press Circle to close");
}
