#include "app_context.h"
#include "scene.h"

static s64 scroll = 0;
static std::filesystem::path panel_dir;
static std::filesystem::file_time_type folder_time{};

void PanelBrowserScene::Enter(AppContext& ctx) {
    panel_dir = WORKDIR_ROOT / "panels";
    if (!std::filesystem::exists(panel_dir)) {
        std::filesystem::create_directories(panel_dir);
    }
    panels_.clear();
    for (auto const& entry : std::filesystem::directory_iterator(panel_dir)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (entry.path().extension() != ".panel") {
            continue;
        }

        panels_.push_back({entry.path(), entry.path().stem().string()});
    }
    std::sort(panels_.begin(), panels_.end(), [](PanelEntry const& a, PanelEntry const& b) {
        return a.display_name < b.display_name;
    });
}

void PanelBrowserScene::Leave(AppContext& ctx) {}

void PanelBrowserScene::Update(AppContext& ctx, double dt) {
    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
        return;
    }
    if (ctx.mice[0].wheel > 0) {
        scroll--;
    }
    if (ctx.mice[0].wheel < 0) {
        scroll++;
    }
    scroll = std::clamp(scroll, (s64)0, std::max((s64)12, (s64)panels_.size()) - 12);

    auto current = std::filesystem::last_write_time(panel_dir);
    if (current != folder_time) {
        folder_time = current;
        Leave(ctx);
        Enter(ctx);
    }
}

void PanelBrowserScene::Draw(AppContext& ctx) {
    UI ui{ctx};
    ui.Label({720, 80}, "Available Panels: " + std::to_string(panels_.size()));
    ui.Label({720, 180}, "scroll: " + std::to_string(scroll));

    VerticalLayout layout = ui.VerticalLayoutPanel({100, 50, 600, 870}, 10, 65, 10);

    for (auto i = scroll; i < std::min(scroll + 12, (s64)panels_.size()); i++) {
        auto const& panel = panels_[i];
        auto rect = layout.Next();

        if (ui.Button(rect, panel.display_name)) {
            ctx.scenes.Push(std::make_unique<PanelViewerScene>(panel.path));
        }
    }
}