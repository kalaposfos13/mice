#include <filesystem>
#include "app_context.h"
#include "scene.h"
#include "ui/widget_manager.h"

PanelViewerScene::PanelViewerScene(std::filesystem::path panel_path)
    : panel_path_(std::move(panel_path)) {}

void PanelViewerScene::Enter(AppContext&) {
    wm_.Load(panel_path_);
}

void PanelViewerScene::Leave(AppContext&) {}

void PanelViewerScene::Update(AppContext& ctx, double) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
        return;
    }

    wm_.Update();
}

void PanelViewerScene::Draw(AppContext& ctx) {
    UI ui{ctx};

    wm_.Draw(ui);
}