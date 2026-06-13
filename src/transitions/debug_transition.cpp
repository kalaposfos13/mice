#include "transition.h"

#include <algorithm>

#include "app_context.h"
#include "ui.h"

float DebugTransition::Duration() const {
    return 0.5f;
}

void DebugTransition::DrawOut(AppContext& ctx, const Image& image, float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    ctx.renderer.DrawImage(image, 0, 0);

    UI ui{ctx};
    auto debug_text = fmt::format("Fade out t: {}", t);
    ui.Label({100, 100}, debug_text);
}

void DebugTransition::DrawIn(AppContext& ctx, const Image& image, float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    ctx.renderer.DrawImage(image, 0, 0);

    UI ui{ctx};
    auto debug_text = fmt::format("Fade in t: {}", t);
    ui.Label({100, 200}, debug_text);
}