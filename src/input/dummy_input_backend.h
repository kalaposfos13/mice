#pragma once

#include "input_backend.h"

class DummyInputBackend : public InputBackend {
    void Init();
    void Start();
    void Stop();

    void Flush();

    void SetPosition(Vec2 const pos);

    bool IsAvailable();
    std::string GetName();
};