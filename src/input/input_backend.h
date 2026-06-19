#pragma once

#include <string>
#include "maths.h"

enum ButtonMask : u32 {
    bNone = 0,
    bPrimary = 1 << 0,
    bSecondary = 1 << 1,
    bTertiary = 1 << 2,
    bExtra1 = 1 << 3,
    bExtra2 = 1 << 4,
};

struct InputButton {
    ButtonMask val;
    InputButton() : val(ButtonMask::bNone) {}
    InputButton(auto v) : val(static_cast<ButtonMask>(v)) {}
    operator ButtonMask&() {
        return val;
    }
    operator ButtonMask const&() const {
        return val;
    }
    bool Includes(ButtonMask m) {
        return val & m;
    }
};

struct InputState {
    InputButton held, pressed, released;
    s32 scroll;
    Vec2 position, movement;
    bool connected;
};

class InputBackend {
public:
    virtual ~InputBackend() = default;
    virtual void Init() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    virtual void Flush() = 0;

    virtual void SetPosition(u64 which, Vec2 where) = 0;

    virtual bool IsAvailable() = 0;
    virtual std::string GetName() = 0;

    std::array<InputState, 2> devices{};
};
