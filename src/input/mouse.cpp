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
                    s32 ret = sceMouseRead(m_handles[m], m_data_bufs[m], 64);
                    if (ret == ORBIS_MOUSE_ERROR_INVALID_HANDLE) {
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
                        }
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(4));
        }
    }};
    reader.detach();
}

void Mice::UpdateState() {
    std::scoped_lock l{mm};
    stable_frame_state = delta_frame_state;
    delta_frame_state = {};
    positions[0].x += stable_frame_state[0].dx;
    positions[0].y += stable_frame_state[0].dy;
    positions[1].x += stable_frame_state[1].dx;
    positions[1].y += stable_frame_state[1].dy;
    return;
}

Mice::~Mice() {
    sceMouseClose(m_handles[0]);
    sceMouseClose(m_handles[1]);
    sceSysmoduleUnloadModule(ORBIS_SYSMODULE_MOUSE);
}
