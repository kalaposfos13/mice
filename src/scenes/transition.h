#pragma once

#include "image.h"

class AppContext;

class Transition {
public:
    virtual ~Transition() = default;
    virtual void DrawOut(AppContext& ctx, Image const& image, float t) = 0;
    virtual void DrawIn(AppContext& ctx, Image const& image, float t) = 0;
    virtual float Duration() const = 0;
};

class FadeTransition : public Transition {
public:
    void DrawOut(AppContext& ctx, Image const& image, float t);
    void DrawIn(AppContext& ctx, Image const& image, float t);
    float Duration() const;
};