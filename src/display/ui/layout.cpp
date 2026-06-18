#include "layout.h"

Rect VerticalLayout::Next() {
    Rect r{x, y, width, height};

    y += height + spacing;

    return r;
}

Rect HorizontalLayout::Next() {
    Rect r{x, y, width, height};

    x += width + spacing;

    return r;
}