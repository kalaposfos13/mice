#include "app_context.h"
#include "scene.h"
#include "ui.h"

#define THIS_SCENE AboutOverlayScene

void THIS_SCENE::Enter(AppContext&) {}

void THIS_SCENE::Leave(AppContext&) {}

void THIS_SCENE::Update(AppContext& ctx, float) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
}

void THIS_SCENE::Draw(AppContext& ctx) {

    UI ui{ctx};

    ui.Label({700, 350}, "M.I.C.E.");
    ui.Label({600, 450}, "Overlay test scene");
    ui.Label({550, 550}, "Press Circle to close");
}
