#include "ui.h"
#include "widget_manager.h"
#include "common/logging.h"

bool WidgetManager::Load(std::filesystem::path const& path) {
    path_ = path;

    Reload();

    return true;
}

void WidgetManager::Reload() {
    PanelLoader parser;

    std::vector<WidgetDefinition> new_widgets;

    if (!parser.Load(path_, new_widgets)) {
        widgets_ = std::move(new_widgets);
        states_.resize(widgets_.size());

        return;
    }
    std::unordered_map<std::string, WidgetState> old_state;
    for (size_t i = 0; i < widgets_.size(); i++) {
        old_state.emplace(widgets_[i].id, states_[i]);
    }
    widgets_ = std::move(new_widgets);

    states_.resize(widgets_.size());
    file_time_ = std::filesystem::last_write_time(path_);
}

void WidgetManager::Update() {
    auto current = std::filesystem::last_write_time(path_);

    if (current != file_time_) {
        Reload();
    }
}

void WidgetManager::Bind(std::string const& id, WidgetState* value) {
    bindings_[id] = value;
}

void WidgetManager::SyncFromBindings() {
    for (size_t i = 0; i < widgets_.size(); i++) {
        auto binding = bindings_.find(widgets_[i].id);

        if (binding == bindings_.end()) {
            continue;
        }

        auto& state = states_[i];
        if (auto ptr = &binding->second) {
            state = **ptr;
        }
    }
}
void WidgetManager::SyncToBindings() {
    for (size_t i = 0; i < widgets_.size(); i++) {
        auto binding = bindings_.find(widgets_[i].id);

        if (binding == bindings_.end()) {
            continue;
        }

        auto& state = states_[i];
        if (auto ptr = &binding->second) {
            **ptr = state;
        }
    }
}

void WidgetManager::DrawWidget(UI& ui, WidgetDefinition& def, WidgetState& state) {
    switch (def.type) {
    case WidgetType::Button: {
        state = ui.Button(def.rect, def.text);
        break;
    }
    case WidgetType::Label:
        ui.Label({def.rect.x, def.rect.y}, def.text);
        break;
    case WidgetType::Checkbox: {
        state = ui.Checkbox(def.rect, state.bool_value, def.text);
        break;
    }
    case WidgetType::Slider: {
        state =  ui.Slider(def.rect, state.float_value, def.min, def.max, def.step);
        break;
    }
    case WidgetType::ProgressBar:
        ui.ProgressBar(def.rect, state.float_value);
        break;
    case WidgetType::Separator:
        ui.Separator(def.rect);
        break;
    case WidgetType::Panel:
        ui.Panel(def.rect);
        break;
    }
}

void WidgetManager::Draw(UI& ui) {
    SyncFromBindings();

    for (size_t i = 0; i < widgets_.size(); i++) {
        DrawWidget(ui, widgets_[i], states_[i]);
    }

    SyncToBindings();
}
