#pragma once

#include "common/types.h"
#include "input/mouse.h"
#include "display/graphics.h"

namespace Colors {
static constexpr Color red{255, 0, 0};
static constexpr Color green{0, 175, 0};
static constexpr Color blue{0, 0, 255};
static constexpr Color black{0, 0, 0};
static constexpr Color white{255, 255, 255};
static constexpr Color dim_white{200, 200, 200};

static constexpr Color bright_yellow{255, 255, 120};

static constexpr Color orange{255, 140, 0};
static constexpr Color bright_orange{255, 200, 80};

static constexpr Color cyan{0, 180, 255};
static constexpr Color bright_cyan{120, 220, 255};

static constexpr Color silver{170, 170, 170};
static constexpr Color darker_silver{150, 150, 150};

static constexpr Color gray{100, 100, 100};
static constexpr Color background{50, 50, 50};
} // namespace Colors

struct UITheme {
    Color panel_fill;
    Color panel_border;

    Color control_fill;
    Color control_border;

    Color text;
    Color text_outline;

    Color accent;
    Color accent_secondary;

    Color separator;

    Color hover_m0;
    Color hover_m1;
    Color dual_hover;

    Color active_m0;
    Color active_m1;
};

struct Point {
    s32 x{};
    s32 y{};
};

struct Rect {
    s32 x{};
    s32 y{};
    s32 w{};
    s32 h{};

    bool Contains(s32 const _x, s32 const _y) const {
        return _x > x && _x < x + w && _y > y && _y < y + h;
    }
    bool Contains(MousePosition const p) const {
        return Contains(p.x, p.y);
    }
};

enum class FontSize {
    Small = 0,
    Medium,
    Large,
};

struct WidgetState {
    bool hovered{};
    bool held{};
    bool clicked{};
    bool released{};

    bool hovered_m0{};
    bool hovered_m1{};

    bool held_m0{};
    bool held_m1{};

    bool clicked_m0{};
    bool clicked_m1{};

    bool released_m0{};
    bool released_m1{};

    explicit operator bool() const {
        return released;
    }
};

struct CheckboxState {
    WidgetState interaction;
    bool value{};
};
struct SliderState {
    WidgetState interaction;
    float value{};
};
