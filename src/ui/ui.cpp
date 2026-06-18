#include "app_context.h"
#include "common/logging.h"
#include "ui.h"

UI::UI() {}

FT_Face UI::fonts[3]{};
void UI::InitFonts() {
    std::string font_path = APP_ROOT / "assets/fonts/Monocraft.ttf";
    for (int i = 0; i < 3; i++) {
        if (ctx.renderer.scene->InitFont(&fonts[i], font_path.c_str(),
                                         GetFontActualSize((FontSize)i)) &&
            fonts[i] == nullptr) {
            LOG_ERROR("Failed to init font");
        }
    }
}

s32 UI::GetFontActualSize(FontSize const s) {
    return 20 + (u32)s * 15;
}

UITheme UI::default_theme = {
    .panel_fill = Colors::background,
    .panel_border = Colors::silver,

    .control_fill = Colors::silver,
    .control_border = Colors::darker_silver,

    .text = Colors::black,
    .text_outline = Colors::black,

    .accent = Colors::green,
    .accent_secondary = Colors::bright_yellow,

    .separator = Colors::gray,

    .hover_m0 = Colors::orange,
    .hover_m1 = Colors::cyan,
    .dual_hover = Colors::bright_yellow,

    .active_m0 = Colors::bright_orange,
    .active_m1 = Colors::bright_cyan,
};

WidgetState UI::Evaluate(Rect const& rect) {
    WidgetState s{};

    s.hovered_m0 = rect.Contains(ctx.mice[0].position);
    s.hovered_m1 = rect.Contains(ctx.mice[1].position);

    bool accept = ctx.accepting_inputs;

    s.held_m0 = accept && s.hovered_m0 && ctx.mice[0].current_buttons.Includes(MouseButton::Right);

    s.held_m1 = accept && s.hovered_m1 && ctx.mice[1].current_buttons.Includes(MouseButton::Left);

    s.clicked_m0 =
        accept && s.hovered_m0 && ctx.mice[0].clicked_buttons.Includes(MouseButton::Right);

    s.clicked_m1 =
        accept && s.hovered_m1 && ctx.mice[1].clicked_buttons.Includes(MouseButton::Left);

    s.released_m0 =
        accept && s.hovered_m0 && ctx.mice[0].released_buttons.Includes(MouseButton::Right);

    s.released_m1 =
        accept && s.hovered_m1 && ctx.mice[1].released_buttons.Includes(MouseButton::Left);

    s.hovered = s.hovered_m0 || s.hovered_m1;

    s.held = s.held_m0 || s.held_m1;

    s.clicked = s.clicked_m0 || s.clicked_m1;

    s.released = s.released_m0 || s.released_m1;

    return s;
}

static Color GetMainColorForState(WidgetState const& state, UITheme const& theme) {
    Color ret = theme.control_border;

    if (state.hovered_m0 && state.hovered_m1) {
        ret = state.held_m0 && state.held_m1 ? Colors::white
              : state.held_m0                ? theme.active_m0
              : state.held_m1                ? theme.active_m1
                                             : theme.dual_hover;
    } else if (state.hovered_m0) {
        ret = state.held_m0 ? theme.active_m0 : theme.hover_m0;
    } else if (state.hovered_m1) {
        ret = state.held_m1 ? theme.active_m1 : theme.hover_m1;
    }

    return ret;
}

WidgetState UI::Button(Rect const& rect, std::string_view text, FontSize size) {
    WidgetState state = Evaluate(rect);

    Color outline = GetMainColorForState(state, theme_);

    ctx.renderer.scene->DrawRectangleWithBorder(rect.x, rect.y, rect.w, rect.h, theme_.control_fill,
                                                10, outline);

    ctx.renderer.scene->DrawText(text.data(), fonts[(u32)size], rect.x + 15,
                                 rect.y + 15 + GetFontActualSize(size), theme_.control_fill,
                                 theme_.text);

    return state;
}

void UI::Label(Point pos, std::string_view const text, FontSize const size) {
    ctx.renderer.scene->DrawText(text.data(), fonts[(u32)size], pos.x + 7,
                                 pos.y + 7 + GetFontActualSize(size), theme_.panel_fill,
                                 theme_.text);
}

void UI::DrawCursors() {
    MousePosition const& mps0 = ctx.mice[0].position;
    MousePosition const& mps1 = ctx.mice[1].position;
    bool m0_pressed = ctx.mice[0].current_buttons.Includes(MouseButton::Right);
    bool m1_pressed = ctx.mice[1].current_buttons.Includes(MouseButton::Left);

    auto sc = ctx.renderer.scene;
    sc->DrawRectangle(mps0.x - 25, mps0.y, 25, 25,
                      m0_pressed ? Colors::bright_orange : Colors::orange);
    sc->DrawRectangle(mps0.x - 5, mps0.y, 5, 5, Colors::white);

    sc->DrawRectangle(mps1.x, mps1.y, 25, 25, m1_pressed ? Colors::bright_cyan : Colors::cyan);
    sc->DrawRectangle(mps1.x, mps1.y, 5, 5, Colors::white);
}

void UI::Panel(Rect const& rect, Color fill, Color border) {
    ctx.renderer.scene->DrawRectangleWithBorder(rect.x, rect.y, rect.w, rect.h, fill, 10, border);
}

VerticalLayout UI::VerticalLayoutPanel(Rect const& panel, s32 padding, s32 item_height,
                                       s32 spacing) {
    Panel(panel);
    padding += 10;
    return VerticalLayout{panel.x + padding, panel.y + padding, panel.w - padding * 2, item_height,
                          spacing};
}

HorizontalLayout UI::HorizontalLayoutPanel(Rect const& panel, s32 padding, s32 item_width,
                                           s32 spacing) {
    Panel(panel);
    padding += 10;
    return HorizontalLayout{panel.x + padding, panel.y + padding, item_width, panel.h - padding * 2,
                            spacing};
}

WidgetState UI::Checkbox(Rect const& rect, bool value, std::string_view label) {
    auto state = Evaluate(rect);

    const int box_size = rect.h;
    Rect box{rect.x, rect.y, box_size, box_size};
    Rect label_r{rect.x + box_size + 15, rect.y, rect.w - box_size - 15, rect.h};

    Color fill = value ? state.held ? theme_.accent_secondary : theme_.accent : theme_.control_fill;

    ctx.renderer.scene->DrawRectangleWithBorder(box.x, box.y, box.w, box.h, theme_.control_fill, 4,
                                                GetMainColorForState(state, theme_));

    if (value) {
        ctx.renderer.scene->DrawRectangle(box.x + 8, box.y + 8, box.w - 16, box.h - 16, fill);
    }

    if (state.released) {
        value = !value;
    }

    if (!label.empty()) {
        Label({label_r.x + 10, label_r.y + 4}, label);
    }

    state.bool_value = value;

    return state;
}
WidgetState UI::Slider(Rect const& rect, float value, float min, float max, float step) {
    auto state = Evaluate(rect);

    Rect track = rect;
    track.x += 8;
    track.w -= 16;

    bool dragging = state.held_m0 || state.held_m1;

    if (dragging) {

        int x;

        if (state.held_m0)
            x = ctx.mice[0].position.x;
        else
            x = ctx.mice[1].position.x;

        float t = float(x - track.x) / float(track.w);

        t = std::clamp(t, 0.0f, 1.0f);

        value = min + t * (max - min);

        if (step > 0.0f) {
            value = std::round(value / step) * step;
        }
    }

    ctx.renderer.scene->DrawRectangleWithBorder(rect.x, rect.y, rect.w, rect.h, theme_.control_fill,
                                                3, GetMainColorForState(state, theme_));

    float t = (value - min) / (max - min);

    int knob_x = track.x + int(t * track.w);

    auto knob_c = state.hovered ? theme_.accent_secondary : theme_.accent;

    ctx.renderer.scene->DrawRectangle(knob_x - 10, rect.y - 5, 20, rect.h + 10, knob_c);

    state.float_value = value;

    return state;
}

void UI::Separator(s32 x, s32 y, s32 width) {
    ctx.renderer.scene->DrawRectangle(x, y, width, 2, theme_.separator);
}

void UI::Separator(Rect r) {
    ctx.renderer.scene->DrawRectangle(r.x, r.y + r.h / 2, r.w, 2, theme_.separator);
}

void UI::ProgressBar(Rect const& rect, float value) {
    value = std::clamp(value, 0.0f, 1.0f);

    ctx.renderer.scene->DrawRectangleWithBorder(rect.x, rect.y, rect.w, rect.h, theme_.control_fill,
                                                3, theme_.control_fill);

    ctx.renderer.scene->DrawRectangle(rect.x + 3, rect.y + 3, int((rect.w - 6) * value), rect.h - 6,
                                      theme_.accent);
}