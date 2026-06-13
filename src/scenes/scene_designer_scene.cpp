#include "app_context.h"
#include "scene.h"
#include "ui/parser.h"

#define THIS_SCENE SceneDesignerScene

static std::vector<WidgetDefinition> widgets;
static std::filesystem::file_time_type panel_file_time;

static void DrawWidget(UI& ui, WidgetDefinition& w) {
    switch (w.type) {

    case WidgetType::Button:
        ui.Button(w.rect, w.text);
        break;

    case WidgetType::Label:
        ui.Label({w.rect.x, w.rect.y}, w.text);
        break;

    case WidgetType::Checkbox:
        w.value = (s32)ui.Checkbox(w.rect, w.value == 0.0f ? false : true, w.text).value;
        break;

    case WidgetType::Slider:
        w.value = ui.Slider(w.rect, w.value, w.min, w.max, w.step).value;
        break;

    case WidgetType::ProgressBar:
        ui.ProgressBar(w.rect, w.value);
        break;

    case WidgetType::Separator:
        ui.Separator(w.rect);
        break;

    case WidgetType::Panel:
        ui.Panel(w.rect);
        break;
    }
}

void THIS_SCENE::Enter(AppContext& ctx) {
    LOG_INFO("enter");
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2 + 300);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2 + 300);

    PanelLoader p{};
    p.Load(WORKDIR_ROOT / "panel.txt", widgets);
    panel_file_time = std::filesystem::last_write_time(WORKDIR_ROOT / "panel.txt");
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
    auto new_file_time = std::filesystem::last_write_time(WORKDIR_ROOT / "panel.txt");
    if (panel_file_time != new_file_time) {
        PanelLoader p{};
        p.Load(WORKDIR_ROOT / "panel.txt", widgets);
        LOG_INFO("panel reloaded");
    }
    panel_file_time = new_file_time;
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};
    for (auto& w : widgets) {
        DrawWidget(ui, w);
    }
}
