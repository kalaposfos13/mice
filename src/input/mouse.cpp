#include <thread>
#include <orbis/Sysmodule.h>
#include "app_context.h"
#include "common/assert.h"
#include "mouse.h"

void Mice::Init() {
    static bool lib_loaded = false;
    if (!lib_loaded) {
        sceSysmoduleLoadModule(ORBIS_SYSMODULE_MOUSE);
        sceMouseInit();
        lib_loaded = true;
    }
}

Mice::Mice() {
    Init();
}

static inline u32 SwapPrimarySecondary(u32 bits) {
    constexpr u32 mask = bPrimary | bSecondary;

    u32 pair = bits & mask;

    bits &= ~mask;

    bits |= ((pair & bPrimary) << 1);
    bits |= ((pair & bSecondary) >> 1);

    return bits;
}

void Mice::Start() {
    OrbisMouseOpenParam p{.flag = MouseOpenBehaviour::Normal};
    ASSERT_NO_ERROR(mice[0].handle = sceMouseOpen(ctx.user_id, 0, 0, &p));
    ASSERT_NO_ERROR(mice[1].handle = sceMouseOpen(ctx.user_id, 0, 1, &p));
    LOG_INFO("m0 handle: {}, m1 handle: {}", mice[0].handle, mice[1].handle);
    std::thread reader{[this]() {
        while (true) {
            {
                std::scoped_lock l{mm};
                for (Mouse& mouse : mice) {
                    s32 ret = sceMouseRead(mouse.handle, mouse.data_buf.data(), 64);
                    if (ret == ORBIS_MOUSE_ERROR_INVALID_HANDLE) {
                        LOG_INFO("Exiting mouse read thread");
                        mice[0].handle = -1;
                        mice[1].handle = -1;
                        return;
                    }
                    ASSERT_NO_ERROR(ret);
                    {
                        MouseFrameState& acc = mouse.delta;

                        for (int i = 0; i < ret; i++) {
                            const auto& d = mouse.data_buf[i];
                            if (!d.connected) {
                                LOG_INFO("Mouse not connected");
                                mice[0].handle = -1;
                                mice[1].handle = -1;
                                return;
                            }

                            acc.dx += d.x_axis;
                            acc.dy += d.y_axis;
                            acc.wheel += d.wheel;
                            acc.tilt += d.tilt;

                            acc.buttons.bits |= mouse.handle == mice[0].handle
                                                    ? SwapPrimarySecondary(d.buttons)
                                                    : d.buttons;
                            acc.timestamp = d.timestamp;
                        }
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
        }
    }};
    reader.detach();
}

void Mice::Stop() {
    std::scoped_lock l{mm};
    mice[0].handle = -1;
    mice[1].handle = -1;
}

void Mice::Flush() {
    std::scoped_lock l{mm};
    for (Mouse& mouse : mice) {
        if (mouse.delta.timestamp == 0) {
            mouse.clicked_buttons = {};
            mouse.released_buttons = {};
            mouse.wheel = 0;
            continue; // no update came
        }
        mouse.stable = mouse.delta;
        mouse.delta = {};
        mouse.wheel = mouse.stable.wheel;
        mouse.position.x += mouse.stable.dx * 2; // arbitrary scaling, todo fix
        mouse.position.y += mouse.stable.dy * 2;
        mouse.position.x = std::clamp((s32)mouse.position.x, 0, 1920);
        mouse.position.y = std::clamp((s32)mouse.position.y, 0, 1080);
        auto old = mouse.current_buttons;
        auto now = mouse.stable.buttons;
        mouse.current_buttons = now;
        mouse.clicked_buttons = now.bits & ~old.bits;
        mouse.released_buttons = old.bits & ~now.bits;
    }
    devices[0] = mice[0];
    devices[1] = mice[1];
}

Mice::~Mice() {
    for (Mouse& mouse : mice) {
        sceMouseClose(mouse.handle);
    }
}

void Mice::SetPosition(u64 which, Vec2 const pos) {
    mice[which].position.x = pos.x;
    mice[which].position.y = pos.y;
}

bool Mice::IsAvailable() {
    if (mice[0].handle >= 0) {
        return true;
    }
    OrbisMouseOpenParam p{.flag = MouseOpenBehaviour::Normal};
    s32 temp_handle1 = sceMouseOpen(ctx.user_id, 0, 0, &p);
    s32 temp_handle2 = sceMouseOpen(ctx.user_id, 0, 1, &p);
    bool ret = false;
    if (temp_handle1 >= 0 && temp_handle2 >= 0) {
        OrbisMouseData md{};
        md.connected = true;
        LOG_VALUE(sceMouseRead(temp_handle1, &md, 1));
        bool first_connected = md.connected;
        md.connected = true;
        LOG_VALUE(sceMouseRead(temp_handle2, &md, 1));
        bool second_connected = md.connected;
        ret = first_connected && second_connected;
    }
    sceMouseClose(temp_handle1);
    sceMouseClose(temp_handle2);
    return ret;
}

Mouse::operator InputState() const {
    return InputState{
        .held = u32(current_buttons),
        .pressed = u32(clicked_buttons),
        .released = u32(released_buttons),
        .scroll = wheel,
        .position = position,
        .movement = {stable.dx, stable.dy},
        .connected = handle != -1,
    };
}
