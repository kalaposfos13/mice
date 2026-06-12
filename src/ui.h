#pragma once

#include "app_context.h"

namespace Colors {
static constexpr Color red{255, 0, 0};
static constexpr Color green{0, 255, 0};
static constexpr Color blue{0, 0, 255};
static constexpr Color black{0, 0, 0};
static constexpr Color white{255, 255, 255};
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

struct Rect {
    s32 x, y, w, h;

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

enum class ButtonReport {
    Never = 0,
    Hovered,
    Held,
    Clicked,
    Released,
};

class UI {
    static s32 GetFontActualSize(FontSize const s);
public:
    explicit UI(AppContext& _ctx);
    static void InitFonts(AppContext& ctx);

    bool Button(Rect const& rect, std::string_view const text, FontSize const size, ButtonReport report);

    void Label(Rect const& rect, std::string_view const text, FontSize const size);

    static void DrawCursors(AppContext& ctx);

private:
    AppContext& ctx;
    static FT_Face fonts[3];
};