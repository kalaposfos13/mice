#include "app_context.h"
#include "game_scene.h"

#define THIS_SCENE GameScene

GameScene::GameScene(SaveData save) : game(save) {}

void THIS_SCENE::Enter() {
    ctx.state.draw_mice = false;
}

void THIS_SCENE::Leave() {
    ctx.state.draw_mice = true;
}

void THIS_SCENE::Update(double dt) {
    game.Update(dt);
}

void THIS_SCENE::Draw() {
    game.Draw();
}
