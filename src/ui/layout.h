#pragma once

#include "ui_types.h"

class Layout {
public:
    virtual ~Layout() = default;

    virtual Rect Next() = 0;
};

class VerticalLayout final : public Layout {
public:
    VerticalLayout(s32 x, s32 y, s32 width, s32 height, s32 spacing)
        : x(x), y(y), width(width), height(height), spacing(spacing) {};

    Rect Next() override;

private:
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 spacing;
};

class HorizontalLayout final : public Layout {
public:
    HorizontalLayout(s32 x, s32 y, s32 width, s32 height, s32 spacing)
        : x(x), y(y), width(width), height(height), spacing(spacing) {};

    Rect Next() override;

private:
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 spacing;
};