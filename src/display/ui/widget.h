#pragma once

#include <string>
#include "ui_types.h"

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
    std::string id;

    bool bool_value{};

    float value{};
    float min{};
    float max{};
    float step{};
};