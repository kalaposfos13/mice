#include "transition.h"

#include <algorithm>

#include "app_context.h"

float FadeTransition::Duration() const {
    return 0.1f;
}

void FadeTransition::DrawOut(const Image& image, float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    ctx.renderer.DrawImageDarkened(image, 0, 0, 1 - t);
}

void FadeTransition::DrawIn(const Image& image, float t) {
    t = std::clamp(t, 0.0f, 1.0f);

    ctx.renderer.DrawImageDarkened(image, 0, 0, t);
}