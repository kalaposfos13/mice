#include <thread>
#include <orbis/Sysmodule.h>
#include "common/assert.h"
#include "mouse.h"

void Mice::Init(OrbisUserServiceUserId const& uid) {
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_MOUSE);
    sceMouseInit();
    OrbisMouseOpenParam p{.flag = MouseOpenBehaviour::Normal};
    m_handles[0] = sceMouseOpen(uid, 0, 0, &p);
    m_handles[1] = sceMouseOpen(uid, 0, 1, &p);
    LOG_INFO("m0 handle: {}, m1 handle: {}", m_handles[0], m_handles[1]);
    std::thread reader{[this]() {
        while (true) {
            {
                std::scoped_lock l{mm};
                for (int m = 0; m < 2; m++) {
                    s32 ret = sceMouseRead(m_handles[m], m_data_bufs[m].data(), 64);
                    if (ret == ORBIS_MOUSE_ERROR_INVALID_HANDLE) {
                        LOG_INFO("Exiting mouse read thread");
                        return;
                    }
                    ASSERT_NO_ERROR(ret);
                    {
                        MouseFrameState& acc = delta_frame_state[m];

                        for (int i = 0; i < ret; i++) {
                            const auto& d = m_data_bufs[m][i];

                            acc.dx += d.x_axis;
                            acc.dy += d.y_axis;
                            acc.wheel += d.wheel;
                            acc.tilt += d.tilt;

                            acc.buttons |= d.buttons;
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

void Mice::UpdateState() {
    std::scoped_lock l{mm};
    for (int m = 0; m < 2; m++) {
        if (delta_frame_state[m].timestamp == 0) {
            clicked_btns[m] = {};
            unpressed_btns[m] = {};
            continue; // no update came
        }
        stable_frame_state[m] = delta_frame_state[m];
        delta_frame_state[m] = {};
        positions[m].x += stable_frame_state[m].dx;
        positions[m].y += stable_frame_state[m].dy;
        positions[m].x += stable_frame_state[m].dx;
        positions[m].y += stable_frame_state[m].dy;
        auto old = current_btns[m];
        auto now = stable_frame_state[m].buttons;
        current_btns[m] = now;
        clicked_btns[m] = now & ~old;
        unpressed_btns[m] = old & ~now;
    }
}

Mice::~Mice() {
    sceMouseClose(m_handles[0]);
    sceMouseClose(m_handles[1]);
    sceSysmoduleUnloadModule(ORBIS_SYSMODULE_MOUSE);
}

void Mice::SetCursor(s32 which, s32 x, s32 y) {
    positions[which].x = x;
    positions[which].y = y;
}