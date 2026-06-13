#include "transition.h"

#include <algorithm>

#include "app_context.h"
#include "renderer.h"
#include "ui.h"

float FadeTransition::Duration() const {
    return 0.25f;
}

void FadeTransition::DrawOut(
    AppContext& ctx,
    const Image& image,
    float t)
{
    t = std::clamp(t, 0.0f, 1.0f);

    UI ui{ctx};
    auto debug_text = fmt::format("Fade out t: {}", t);
    ui.Label({100, 100}, debug_text);
}

void FadeTransition::DrawIn(
    AppContext& ctx,
    const Image& image,
    float t)
{
    t = std::clamp(t, 0.0f, 1.0f);

    UI ui{ctx};
    auto debug_text = fmt::format("Fade in t: {}", t);
    ui.Label({100, 100}, debug_text);
}