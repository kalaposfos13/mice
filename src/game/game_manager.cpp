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

    LOG_CALL(ctx.input.SetPosition(0, player.arms[0].target));
    ctx.input.SetPosition(1, player.arms[1].target);
}

void GameManager::Update(double dt) {
    for (int i = 0; i < player.arms.size(); i++) {
        player.arms[i].target = {ctx.input[i].position.x, ctx.input[i].position.y};
    }
}

void GameManager::Draw() {
    UI ui{};
    ui.Button({100, 100, 100, 100}, "");

    player.arms[0].Draw(false);
    player.arms[1].Draw(true);
}
