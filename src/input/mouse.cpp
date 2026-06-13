#include <thread>
#include <orbis/Sysmodule.h>
#include "common/assert.h"
#include "mouse.h"

void Mice::Init(OrbisUserServiceUserId const& uid) {
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_MOUSE);
    sceMouseInit();
    OrbisMouseOpenParam p{.flag = MouseOpenBehaviour::Normal};
    mice[0].handle = sceMouseOpen(uid, 0, 0, &p);
    mice[1].handle = sceMouseOpen(uid, 0, 1, &p);
    LOG_INFO("m0 handle: {}, m1 handle: {}", mice[0].handle, mice[1].handle);
    std::thread reader{[this]() {
        while (true) {
            {
                std::scoped_lock l{mm};
                for (Mouse& mouse : mice) {
                    s32 ret = sceMouseRead(mouse.handle, mouse.data_buf.data(), 64);
                    if (ret == ORBIS_MOUSE_ERROR_INVALID_HANDLE) {
                        LOG_INFO("Exiting mouse read thread");
                        return;
                    }
                    ASSERT_NO_ERROR(ret);
                    {
                        MouseFrameState& acc = mouse.delta;

                        for (int i = 0; i < ret; i++) {
                            const auto& d = mouse.data_buf[i];

                            acc.dx += d.x_axis;
                            acc.dy += d.y_axis;
                            acc.wheel += d.wheel;
                            acc.tilt += d.tilt;

                            acc.buttons.bits |= d.buttons;
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

void Mice::Update() {
    std::scoped_lock l{mm};
    for (Mouse& mouse : mice) {
        if (mouse.delta.timestamp == 0) {
            mouse.clicked_buttons = {};
            mouse.released_buttons = {};
            continue; // no update came
        }
        mouse.stable = mouse.delta;
        mouse.delta = {};
        mouse.position.x += mouse.stable.dx;
        mouse.position.y += mouse.stable.dy;
        mouse.position.x += mouse.stable.dx;
        mouse.position.y += mouse.stable.dy;
        auto old = mouse.current_buttons;
        auto now = mouse.stable.buttons;
        mouse.current_buttons = now;
        mouse.clicked_buttons = now.bits & ~old.bits;
        mouse.released_buttons = old.bits & ~now.bits;
    }
}

Mice::~Mice() {
    for (Mouse& mouse : mice) {
        sceMouseClose(mouse.handle);
    }

    sceSysmoduleUnloadModule(ORBIS_SYSMODULE_MOUSE);
}

void Mice::SetCursor(s32 which, s32 x, s32 y) {
    mice[which].position.x = x;
    mice[which].position.y = y;
}

void Mice::Recenter(s32 which) {
    mice[which].position.x = 1920 / 2;
    mice[which].position.y = 1080 / 2;
}