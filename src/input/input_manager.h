#pragma once

#include "input_backend.h"

enum class InputBackendType { Mouse, Keyboard, Move, Controller, None };

class InputManager {
public:
    void Init();
    std::unique_ptr<InputBackend> current_backend;
    void SwitchBackendTo(InputBackendType new_backend);
    void Update();
    InputState operator[](u64 index);
    bool IsBackendConnected() const;
    void Recenter(u64 which);
    void SetPosition(u64 which, Vec2 where);
    bool IsPressed(ButtonMask m);
};
