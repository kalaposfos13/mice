#pragma once

#include <cmath>

class Vec2 {
public:
    double x, y;
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