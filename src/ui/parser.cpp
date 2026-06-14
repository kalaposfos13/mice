#include "common/logging.h"
#include "parser.h"

#include <fstream>
#include <sstream>

Layout* PanelLoader::CurrentLayout() {
    if (layouts.empty()) {
        return nullptr;
    }

    return layouts.back().get();
}

static bool ParseError(std::vector<WidgetDefinition>& widgets, size_t line,
                       std::string const& message) {
    widgets.clear();

    WidgetDefinition w;
    w.type = WidgetType::Label;
    w.rect = {50, 50, 1500, 50};
    w.text = "Line " + std::to_string(line) + ": " + message;

    widgets.push_back(std::move(w));
    return false;
}

bool PanelLoader::Load(std::filesystem::path const& path, std::vector<WidgetDefinition>& widgets) {
    widgets.clear();
    layouts.clear();

    std::ifstream file(path);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open {}", path.string());
        return false;
    }

    std::string line;
    u64 line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;
        if (line.empty()) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd.empty()) {
            continue;
        }
        if (cmd == "PANEL") {
            WidgetDefinition w;
            w.type = WidgetType::Panel;
            ss >> w.id;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y >> w.rect.w >> w.rect.h;
            }

            widgets.push_back(std::move(w));
            continue;
        }
        if (cmd == "LABEL") {
            WidgetDefinition w;
            w.type = WidgetType::Label;
            ss >> w.id;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y;
            }
            std::getline(ss >> std::ws, w.text);

            widgets.push_back(std::move(w));
            continue;
        }
        if (cmd == "BEGIN_VERTICAL") {
            s32 x;
            s32 y;
            s32 width;
            s32 height;
            s32 spacing;

            ss >> x >> y >> width >> height >> spacing;

            layouts.push_back(std::make_unique<VerticalLayout>(x, y, width, height, spacing));
            continue;
        }
        if (cmd == "BEGIN_HORIZONTAL") {
            s32 x;
            s32 y;
            s32 width;
            s32 height;
            s32 spacing;

            ss >> x >> y >> width >> height >> spacing;

            layouts.push_back(std::make_unique<HorizontalLayout>(x, y, width, height, spacing));
            continue;
        }
        if (cmd == "END_LAYOUT") {
            if (layouts.empty()) {
                return ParseError(widgets, line_number, "END_LAYOUT without matching BEGIN");
            }
            layouts.pop_back();
            continue;
        }
        if (cmd == "BUTTON") {
            WidgetDefinition w;
            w.type = WidgetType::Button;
            ss >> w.id;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y >> w.rect.w >> w.rect.h;
            }
            std::getline(ss >> std::ws, w.text);

            widgets.push_back(std::move(w));

            continue;
        }
        if (cmd == "CHECKBOX") {
            WidgetDefinition w;
            w.type = WidgetType::Checkbox;
            ss >> w.id;

            int checked;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y >> w.rect.w >> w.rect.h;
            }
            ss >> checked;
            std::getline(ss >> std::ws, w.text);

            w.bool_value = checked != 0;

            widgets.push_back(std::move(w));

            continue;
        }
        if (cmd == "SLIDER") {
            WidgetDefinition w;
            w.type = WidgetType::Slider;
            ss >> w.id;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y >> w.rect.w >> w.rect.h;
            }
            ss >> w.value >> w.min >> w.max >> w.step;

            widgets.push_back(std::move(w));

            continue;
        }
        if (cmd == "PROGRESSBAR") {
            WidgetDefinition w;
            w.type = WidgetType::ProgressBar;
            ss >> w.id;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y >> w.rect.w >> w.rect.h;
            }
            ss >> w.value;

            widgets.push_back(std::move(w));

            continue;
        }
        if (cmd == "SEPARATOR") {
            WidgetDefinition w;
            w.type = WidgetType::Separator;
            ss >> w.id;

            Layout* layout = CurrentLayout();
            if (layout) {
                w.rect = layout->Next();
            } else {
                ss >> w.rect.x >> w.rect.y >> w.rect.w >> w.rect.h;
            }

            widgets.push_back(std::move(w));

            continue;
        }
        return ParseError(widgets, line_number, "Unknown command '" + cmd + "'");
    }
    if (!layouts.empty()) {
        return ParseError(widgets, line_number, "Missing END_LAYOUT");
    }

    return true;
}