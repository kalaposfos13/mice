#include <memory>
#include "common/assert.h"
#include "input_manager.h"
#include "keyboard.h"
#include "mouse.h"
#include "move.h"
#include "pad.h"

void InputManager::Init() {
    SwitchBackendTo(InputBackendType::Mouse);
    Recenter(0);
    Recenter(1);
}

void InputManager::SwitchBackendTo(InputBackendType new_backend) {
    switch (new_backend) {
    case InputBackendType::Mouse:
        current_backend = std::make_unique<Mice>();
        break;
        // case InputBackendType::Keyboard:
        //     current_backend = std::make_unique<Keyboard>();
        //     break;
        // case InputBackendType::Move:
        //     current_backend = std::make_unique<Moves>();
        //     break;

    default:
        UNREACHABLE_MSG("Invalid backend type {}", u32(new_backend));
    }
    current_backend->Init();
    ASSERT(current_backend->IsAvailable());
    current_backend->Start();
}

void InputManager::Update() {
    current_backend->Flush();
}

InputState InputManager::operator[](u64 index) {
    return current_backend->devices[index];
}

bool InputManager::IsBackendConnected() const {
    return current_backend != nullptr && current_backend->IsAvailable();
}

void InputManager::Recenter(u64 which) {
    SetPosition(which, {1920 / 2, 1080 / 2});
}

void InputManager::SetPosition(u64 which, Vec2 where) {
    if (!current_backend) {
        return;
    }
    current_backend->SetPosition(which, where);
}

bool InputManager::IsPressed(ButtonMask m) {
    if (!current_backend) {
        return false;
    }
    return (current_backend->devices[0].pressed & m) || (current_backend->devices[1].pressed & m);
}