#include "common/logging.h"
#include "ui.h"

UI::UI(AppContext& _ctx) : ctx(_ctx) {}

FT_Face UI::fonts[3]{};
void UI::InitFonts(AppContext& ctx) {
    std::string font_path = APP_ROOT + "assets/fonts/Monocraft.ttf";
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

bool UI::Button(Rect const& rect, std::string_view const text, FontSize const size,
                ButtonReport report) {
    bool m0_inside = rect.Contains(ctx.mice.positions[0]);
    bool m1_inside = rect.Contains(ctx.mice.positions[1]);
    bool m0_clicked = ctx.mice.clicked_btns[0] & MouseButtons::Right && ctx.accepting_inputs;
    bool m1_clicked = ctx.mice.clicked_btns[1] & MouseButtons::Left && ctx.accepting_inputs;
    bool m0_pressed = ctx.mice.current_btns[0] & MouseButtons::Right && ctx.accepting_inputs;
    bool m1_pressed = ctx.mice.current_btns[1] & MouseButtons::Left && ctx.accepting_inputs;
    bool m0_released = ctx.mice.unpressed_btns[0] & MouseButtons::Right && ctx.accepting_inputs;
    bool m1_released = ctx.mice.unpressed_btns[1] & MouseButtons::Left && ctx.accepting_inputs;

    Color outline_c = Colors::darker_silver;
    if (m0_inside && m1_inside) {
        outline_c = m0_pressed && m1_pressed ? Colors::white
                    : m0_pressed             ? Colors::bright_orange
                    : m1_pressed             ? Colors::bright_cyan
                                             : Colors::bright_yellow;
    } else if (m0_inside) {
        outline_c = m0_pressed ? Colors::bright_orange : Colors::orange;
    } else if (m1_inside) {
        outline_c = m1_pressed ? Colors::bright_cyan : Colors::cyan;
    }

    ctx.renderer.scene->DrawRectangleWithBorder(rect.x, rect.y, rect.w, rect.h, Colors::silver, 10,
                                                outline_c);
    ctx.renderer.scene->DrawText(text.data(), fonts[(u32)size], rect.x + 15,
                                 rect.y + 15 + GetFontActualSize(size), Colors::silver,
                                 Colors::black);

    switch (report) {
    case ButtonReport::Never:
        return false;
    case ButtonReport::Hovered:
        return (m0_inside) || (m1_inside);
    case ButtonReport::Held:
        return (m0_inside && m0_pressed) || (m1_inside && m1_pressed);
    case ButtonReport::Clicked:
        return (m0_inside && m0_clicked) || (m1_inside && m1_clicked);
    case ButtonReport::Released:
        return (m0_inside && m0_released) || (m1_inside && m1_released);
    default:
        return false;
    }
}

void UI::Label(Rect const& rect, std::string_view const text, FontSize const size) {
    ctx.renderer.scene->DrawText(text.data(), fonts[(u32)size], rect.x + 7,
                                 rect.y + 7 + GetFontActualSize(size), Colors::background,
                                 Colors::black);
}

void UI::DrawCursors(AppContext& ctx) {
    MousePosition const& mps0 = ctx.mice.positions[0];
    MousePosition const& mps1 = ctx.mice.positions[1];
    bool m0_pressed = ctx.mice.current_btns[0] & MouseButtons::Right;
    bool m1_pressed = ctx.mice.current_btns[1] & MouseButtons::Left;

    auto sc = ctx.renderer.scene;
    sc->DrawRectangle(mps0.x - 25, mps0.y, 25, 25,
                      m0_pressed ? Colors::bright_orange : Colors::orange);
    sc->DrawRectangle(mps0.x - 5, mps0.y, 5, 5, Colors::white);

    sc->DrawRectangle(mps1.x, mps1.y, 25, 25, m1_pressed ? Colors::bright_cyan : Colors::cyan);
    sc->DrawRectangle(mps1.x, mps1.y, 5, 5, Colors::white);
}
