#include <filesystem>
#include <fstream>
#include "app_context.h"
#include "scene.h"

PanelViewerScene::PanelViewerScene(std::filesystem::path panel_path)
    : panel_path_(std::move(panel_path)) {}

void PanelViewerScene::Enter() {
    if (!std::filesystem::exists(panel_path_)) {
        std::filesystem::create_directories(panel_path_.parent_path());
        std::ofstream ofs{panel_path_};
        ofs << "# empty panel file\n";
    }
    wm_.Load(panel_path_);
}

void PanelViewerScene::Leave() {}

void PanelViewerScene::Update(double) {
    wm_.Update();
}

void PanelViewerScene::Draw() {
    UI ui{};

    wm_.Draw(ui);
}