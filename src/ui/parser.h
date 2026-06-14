#pragma once

#include <memory>
#include <string>
#include <vector>

#include <filesystem>
#include "widget.h"
#include "layout.h"

class PanelLoader {
public:
    bool Load(
        std::filesystem::path const& path,
        std::vector<WidgetDefinition>& widgets);

private:
    Layout* CurrentLayout();

    std::vector<std::unique_ptr<Layout>> layouts;
};