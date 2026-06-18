#pragma once

#include "layout.h"

class AppContext;

class UI {
public:
    explicit UI();

    static UITheme default_theme;
    UITheme const& theme() const {
        return theme_;
    }

    static void InitFonts();
    static void DrawCursors();

    WidgetState Button(Rect const& rect, std::string_view text, FontSize size = FontSize::Medium);
    WidgetState Checkbox(Rect const& rect, bool value, std::string_view label = {});
    WidgetState Slider(Rect const& rect, float value, float min, float max, float step = 0);

    void Label(Point pos, std::string_view text, FontSize size = FontSize::Medium);
    void ProgressBar(Rect const& rect, float value);
    void Panel(Rect const& rect, Color fill = Colors::background, Color border = Colors::silver);
    VerticalLayout VerticalLayoutPanel(Rect const& panel, s32 padding, s32 item_height,
                                       s32 spacing);

    HorizontalLayout HorizontalLayoutPanel(Rect const& panel, s32 padding, s32 item_width,
                                           s32 spacing);

    void Separator(s32 x, s32 y, s32 width);
    void Separator(Rect r);

private:
    WidgetState Evaluate(Rect const& rect);

    static s32 GetFontActualSize(FontSize);

    UITheme const& theme_ = default_theme;

    static FT_Face fonts[3];
};