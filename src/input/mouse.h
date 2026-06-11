#pragma once

#include <mutex>
#include "orbis/UserService.h"
#include "common/types.h"

constexpr s32 ORBIS_MOUSE_ERROR_INVALID_ARG = 0x80DF0001;
constexpr s32 ORBIS_MOUSE_ERROR_INVALID_HANDLE = 0x80DF0003;
constexpr s32 ORBIS_MOUSE_ERROR_ALREADY_OPENED = 0x80DF0004;
constexpr s32 ORBIS_MOUSE_ERROR_NOT_INITIALIZED = 0x80DF0005;
constexpr s32 ORBIS_MOUSE_ERROR_FATAL = 0x80DF00FF;

extern "C" {
struct OrbisMouseData {
    u64 timestamp;
    bool connected;
    u32 buttons;
    s32 x_axis;
    s32 y_axis;
    s32 wheel;
    s32 tilt;
    u8 reserve[8];
};

enum class MouseOpenBehaviour : u8 {
    Normal = 0,
    Merged = 1,
};

struct OrbisMouseOpenParam {
    MouseOpenBehaviour flag;
    u8 reserve[7];
};

s32 sceMouseInit();
s32 sceMouseOpen(OrbisUserServiceUserId userId, s32 type, s32 index, OrbisMouseOpenParam* pParam);
int sceMouseClose(s32 handle);
int sceMouseRead(s32 handle, OrbisMouseData* pData, s32 num);
}

struct MouseFrameState {
    s32 dx = 0;
    s32 dy = 0;
    s32 wheel = 0;
    s32 tilt = 0;
    u32 buttons = 0;
};

struct MousePosition {
    s32 x = 0, y = 0;
};

enum MouseButtons : u32 {
    None = 0,
    Left = 1,
    Right = 2,
    Middle = 4,
    Side1 = 8,
    Side2 = 16,
};

using FrameStateA = std::array<MouseFrameState, 2>;
class Mice {
public:
    s32 m_handles[2]{};
    FrameStateA delta_frame_state{};
    FrameStateA stable_frame_state{};
    u32 pressed_btns[2]{};
    u32 unpressed_btns[2]{};
    MousePosition positions[2]{};
    OrbisMouseData m_data_bufs[2][64]{};
    std::mutex mm{};
    Mice() {}
    ~Mice();
    void Init(OrbisUserServiceUserId const& uid);
    void UpdateState();
    void SetCursor(s32 which, s32 x, s32 y);
};