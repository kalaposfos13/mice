#include "app_context.h"
#include "display/ui/ui.h"
#include "game_manager.h"

GameManager::GameManager(SaveData&& save) {
    Level dummy_level{};
    dummy_level.starting_pose = {Arm{{860, 900}, {910, 540}, false},
                                 Arm{{1060, 900}, {1010, 540}, false}};
    player.arms = dummy_level.starting_pose;
    player.arms[0].color = Colors::orange;
    player.arms[1].color = Colors::cyan;
    ctx.mice.SetCursor(0, player.arms[0].target.x, player.arms[0].target.y);
    ctx.mice.SetCursor(1, player.arms[1].target.x, player.arms[1].target.y);
}

void GameManager::Update(double dt) {
    for (int i = 0; i < player.arms.size(); i++) {
        player.arms[i].target = {ctx.mice[i].position.x, ctx.mice[i].position.y};
    }
}

void GameManager::Draw() {
    UI ui{};
    ui.Button({100, 100, 100, 100}, "");

    player.arms[0].Draw(false);
    player.arms[1].Draw(true);
}
