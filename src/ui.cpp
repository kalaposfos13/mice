#include "common/logging.h"
#include "ui.h"

UI::UI(AppContext& _ctx) : ctx(_ctx) {}

FT_Face UI::fonts[3]{};
void UI::InitFonts(AppContext& ctx) {
    std::string font_path = APP_ROOT + "assets/fonts/Monocraft.ttf";
    for (int i = 0; i < 3; i++) {
        if (ctx.renderer.scene->InitFont(&fonts[i], font_path.c_str(), 20 + i * 10) &&
            fonts[i] != nullptr) {
            LOG_ERROR("Failed to init font");
        }
    }
}

bool UI::Button(Rect const& rect, std::string_view const text, FontSize const size) {
    bool m0_inside = rect.Contains(ctx.mice.positions[0]);
    bool m1_inside = rect.Contains(ctx.mice.positions[1]);
    bool m0_clicked = ctx.mice.pressed_btns[0] & MouseButtons::Right;
    bool m1_clicked = ctx.mice.pressed_btns[1] & MouseButtons::Left;
    bool m0_pressed = ctx.mice.stable_frame_state[0].buttons & MouseButtons::Right;
    bool m1_pressed = ctx.mice.stable_frame_state[1].buttons & MouseButtons::Left;

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
                                 rect.y + 15 + (20 + (u32)size * 10), Colors::silver,
                                 Colors::black);

    return (m0_inside && m0_clicked) || (m1_inside && m1_clicked);
}

void UI::Label(Rect const& rect, std::string_view const text, FontSize const size) {
    ctx.renderer.scene->DrawText(text.data(), fonts[(u32)size], rect.x + 7, rect.y + 7,
                                 Colors::background, Colors::black);
}

void UI::DrawCursors(AppContext& ctx) {
    MousePosition const& mps0 = ctx.mice.positions[0];
    MousePosition const& mps1 = ctx.mice.positions[1];
    bool m0_pressed = ctx.mice.stable_frame_state[0].buttons & MouseButtons::Right;
    bool m1_pressed = ctx.mice.stable_frame_state[1].buttons & MouseButtons::Left;

    ctx.renderer.scene->DrawRectangle(mps0.x - 25, mps0.y, 25, 25,
                                      m0_pressed ? Colors::bright_orange : Colors::orange);
    ctx.renderer.scene->DrawRectangle(mps0.x - 5, mps0.y, 5, 5,
                                      m0_pressed ? Colors::white : Colors::bright_yellow);

    ctx.renderer.scene->DrawRectangle(mps1.x, mps1.y, 25, 25,
                                      m1_pressed ? Colors::bright_cyan : Colors::cyan);
    ctx.renderer.scene->DrawRectangle(mps1.x, mps1.y, 5, 5,
                                      m1_pressed ? Colors::white : Colors::bright_yellow);
}
