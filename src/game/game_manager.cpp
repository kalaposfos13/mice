#include "game_manager.h"
#include "display/ui/ui.h"

void GameManager::Update(double dt) {}

void GameManager::Draw() {
    UI ui{};
    ui.Label({300, 300}, "Game", FontSize::Large);
    ui.Button({100, 100, 100, 100}, "");
}
