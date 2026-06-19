#include <algorithm>
#include <cmath>
#include "app_context.h"
#include "arm.h"

static void DrawLine(Vec2 from, Vec2 to, Color c, float thickness = 15.0f) {
    ctx.renderer.scene->DrawLine(from.x, from.y, to.x - from.x, to.y - from.y, thickness, c);
}

void Arm::Draw(bool right_side) {
    Vec2 diff = target - base;

    double actual_dist = diff.Length();

    constexpr double epsilon = 0.001;

    if (actual_dist < epsilon) {
        actual_dist = epsilon;
    }

    double max_reach = upper_length + lower_length;

    double clamped_dist = std::min(actual_dist, max_reach - epsilon);

    double extension = std::max(0.0, actual_dist - max_reach);

    double cos_angle =
        (upper_length * upper_length + clamped_dist * clamped_dist - lower_length * lower_length) /
        (2.0 * upper_length * clamped_dist);

    cos_angle = std::clamp(cos_angle, -1.0, 1.0);

    double angle = std::acos(cos_angle);

    double base_angle = std::atan2(diff.y, diff.x);

    double shoulder_angle = base_angle + (right_side ? angle : -angle);

    Vec2 elbow{base.x + std::cos(shoulder_angle) * upper_length,

               base.y + std::sin(shoulder_angle) * upper_length};

    Vec2 forearm_dir = target - elbow;

    double forearm_len = forearm_dir.Length();

    if (forearm_len > epsilon) {
        forearm_dir /= forearm_len;
    } else {
        forearm_dir = {1.0, 0.0};
    }

    Vec2 wrist = elbow + forearm_dir * (lower_length - 50);

    constexpr double hand_offset = 5.0;

    Vec2 mount = target;

    Vec2 hand_center = mount + forearm_dir * hand_offset;

    DrawLine(base, elbow, color, 15.0f);
    DrawLine(elbow, wrist, color, 15.0f);

    Color extension_c = {
        u8(color.r * 0.75),
        u8(color.g * 0.75),
        u8(color.b * 0.75),
    };

    DrawLine(wrist, target, extension_c, 8.0f);

    constexpr double hand_size = 24.0;

    Rect hand_rect{s32(hand_center.x - hand_size / 2), s32(hand_center.y - hand_size / 2),
                   s32(hand_size), s32(hand_size)};

    ctx.renderer.scene->DrawRectangle(hand_rect.x, hand_rect.y, hand_rect.w, hand_rect.h, color);
}