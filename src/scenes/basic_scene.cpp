#include "app_context.h"
#include "common/logging.h"
#include "scene.h"

static double elapsed_time = 0.0;

void BasicScene::Enter() {
    LOG_INFO("Entered basic scene");
    ctx.input.SetPosition(0, {1920 / 2, 1080 / 2});
    ctx.input.SetPosition(1, {1920 / 2, 1080 / 2});
    elapsed_time = 0;
}

void BasicScene::Leave() {
    LOG_INFO("Exited basic scene");
}

void BasicScene::Update(double dt) {
    elapsed_time += dt;
    if (ctx.input.IsPressed(ButtonMask::bPrimary)) {
        ctx.scenes.Push<BasicScene>();
    }
}

void BasicScene::Draw() {
    UI ui{};
    ui.Label({100, 300}, fmt::format("seconds: {}", std::floor(elapsed_time)));
}
