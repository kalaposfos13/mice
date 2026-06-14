#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "parser.h"

class UI;

enum class BindingMode {
    ReadOnly,
    ReadWrite
};

class WidgetManager {
public:
    void Load(std::filesystem::path const& path);

    void Update();
    void Draw(UI& ui);
    void Bind(std::string const& id, WidgetState* value, BindingMode const mode = BindingMode::ReadWrite);

private:
    void Reload();

    void SyncFromBindings();
    void SyncToBindings();

    void DrawWidget(UI& ui, WidgetDefinition& def, WidgetState& state);

private:
    std::filesystem::path path_;
    std::filesystem::file_time_type file_time_;
    std::vector<WidgetDefinition> widgets_;
    std::vector<WidgetState> states_;
    std::unordered_map<std::string, WidgetState*> rw_bindings_;
    std::unordered_map<std::string, WidgetState*> ro_bindings_;
};