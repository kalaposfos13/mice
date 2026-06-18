#include <filesystem>
#include "app_context.h"
#include "scene.h"

PanelViewerScene::PanelViewerScene(std::filesystem::path panel_path)
    : panel_path_(std::move(panel_path)) {}

void PanelViewerScene::Enter() {
    wm_.Load(panel_path_);
}

void PanelViewerScene::Leave() {}

void PanelViewerScene::Update(double) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
        return;
    }

    wm_.Update();
}

void PanelViewerScene::Draw() {
    UI ui{};

    wm_.Draw(ui);
}