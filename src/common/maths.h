#pragma once

#include "types.h"
#include <cmath>

class Vec2 {
public:
    double x, y;
    Vec2(auto x_, auto y_) : x(x_), y(y_) {}
    Vec2(auto x_) : x(x_), y(0) {}
    Vec2() : x(0), y(0) {}
    Vec2 operator-() const {
        return {-x, -y};
    }
    Vec2 operator+(Vec2 const o) const {
        return {x + o.x, y + o.y};
    }
    Vec2 operator-(Vec2 const o) const {
        return {x - o.x, y - o.y};
    }
    Vec2 operator*(double const o) const {
        return {x * o, y * o};
    }
    Vec2 operator/(double const o) const {
        return {x / o, y / o};
    }
    Vec2& operator/=(double const o) {
        x /= o;
        y /= o;
        return *this;
    }
    double Length() {
        return sqrt(x * x + y * y);
    }
};