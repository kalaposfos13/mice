#pragma once

#include <memory>
#include <string>
#include <vector>

#include <filesystem>
#include "layout.h"

enum class WidgetType {
    Label,
    Button,
    Checkbox,
    Slider,
    ProgressBar,
    Separator,
    Panel,
};

struct WidgetDefinition {
    WidgetType type;

    Rect rect{};

    std::string text;

    bool bool_value{};

    float value{};
    float min{};
    float max{};
    float step{};
};

class PanelLoader {
public:
    bool Load(
        std::filesystem::path const& path,
        std::vector<WidgetDefinition>& widgets);

private:
    Layout* CurrentLayout();

    std::vector<std::unique_ptr<Layout>> layouts;
};