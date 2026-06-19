#include "dummy_input_backend.h"

void DummyInputBackend::Init() {}

void DummyInputBackend::Start() {}

void DummyInputBackend::Stop() {}

void DummyInputBackend::Flush() {}

void DummyInputBackend::SetPosition(Vec2 const pos) {}

bool DummyInputBackend::IsAvailable() {
    return false;
}

std::string DummyInputBackend::GetName() {
    return "Dummy";
}
