#include "app_context.h"
#include "scene.h"
#include "ui/widget_manager.h"

#define THIS_SCENE SceneDesignerScene

static WidgetManager wm;
static WidgetState checkbox_state{};

void THIS_SCENE::Enter(AppContext& ctx) {
    LOG_INFO("enter");
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2 + 300);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2 + 300);

    wm.Load(WORKDIR_ROOT / "panel.txt");
    wm.Bind("chkb1", &checkbox_state);
    wm.Bind("btn1", &checkbox_state);
}

void THIS_SCENE::Leave(AppContext& ctx) {}

void THIS_SCENE::Update(AppContext& ctx, double dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
    if ((ctx.mice[0].clicked_buttons.Includes(MouseButton::Middle)) != 0) {
        ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2 + 300);
    }
    if ((ctx.mice[1].clicked_buttons.Includes(MouseButton::Middle)) != 0) {
        ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2 + 300);
    }
    wm.Update();
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};
    wm.Draw(ui);
}
