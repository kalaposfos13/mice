#include "app_context.h"
#include "scene.h"

#define THIS_SCENE SceneDesignerScene

static WidgetManager wm;
static WidgetState checkbox_state{}, slider_state{}, button_state{}, progressbar_state{};

void THIS_SCENE::Enter() {
    LOG_INFO("enter");
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2 + 300);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2 + 300);

    wm.Load(WORKDIR_ROOT / "panel.txt");
    wm.Bind("btn1", &button_state);
    wm.Bind("chkb1", &checkbox_state, BindingMode::ReadWrite);
    wm.Bind("sl1", &slider_state);
    wm.Bind("prb1", &progressbar_state, BindingMode::ReadWrite);
}

void THIS_SCENE::Leave() {}

void THIS_SCENE::Update(double dt) {
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
    if (button_state.released) {
        checkbox_state.bool_value ^= 1;
    }
    progressbar_state.float_value = 1 - slider_state.float_value;
}

void THIS_SCENE::Draw() {
    UI ui{};
    wm.Draw(ui);
}
