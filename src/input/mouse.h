#pragma once

#include <mutex>
#include <utility>
#include "common/types.h"
#include "input_backend.h"
#include "orbis/UserService.h"

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
s32 sceMouseClose(s32 handle);
s32 sceMouseRead(s32 handle, OrbisMouseData* pData, s32 num);
}

enum class MouseButton : u32 {
    None = 0,
    Left = 1,
    Right = 2,
    Middle = 4,
    Side1 = 8,
    Side2 = 16,
};

struct MouseButtonsState {
    u32 bits{};

    MouseButtonsState(u32 val = 0) : bits(val) {}
    bool Includes(MouseButton btn) const {
        return bits & u32(btn);
    }
    operator u32 const() const {
        return bits;
    }
};

struct MouseFrameState {
    s32 dx = 0;
    s32 dy = 0;
    s32 wheel = 0;
    s32 tilt = 0;
    MouseButtonsState buttons = 0;
    u64 timestamp = 0;
};

struct MousePosition {
    s32 x = 0, y = 0;
};

struct Mouse {
    s32 handle = -1;

    MouseFrameState delta{};
    MouseFrameState stable{};

    MouseButtonsState current_buttons{};
    MouseButtonsState clicked_buttons{};
    MouseButtonsState released_buttons{};

    Vec2 position{};
    s32 wheel{};

    std::array<OrbisMouseData, 64> data_buf{};

    operator InputState() const;
};

using MouseArray = std::array<Mouse, 2>;
class Mice : public InputBackend {
public:
    MouseArray mice;
    std::mutex mm{};
    Mice();
    ~Mice();
    void Init();
    void Start();
    void Stop();
    void Flush();
    void SetPosition(u64 which, Vec2 const pos);
    Mouse& operator[](u64 index) {
        return mice[index];
    }
    bool IsAvailable();
    std::string GetName() {
        return "Mice";
    }
};
