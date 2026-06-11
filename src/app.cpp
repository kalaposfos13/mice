#include "app.h"

#include <map>
#include <utility>

void App::Run() {
    while (HandleControllerInput()) {
        mice.UpdateState();
        renderer.BeginFrame();
        MousePosition const& mps0 = mice.positions[0];
        MousePosition const& mps1 = mice.positions[1];
        renderer.scene->DrawRectangle(mps0.x, mps0.y, 25, 25, {255, 0, 0});
        renderer.scene->DrawRectangle(mps1.x, mps1.y, 25, 25, {0, 255, 0});
        renderer.EndFrame();
    }
}

App::App() {
    OrbisUserServiceInitializeParams param;
    param.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
    sceUserServiceInitialize(&param);
    sceUserServiceGetInitialUser(&user_id);
    scePadInit();
    ASSERT_NO_ERROR(pad_handle = scePadOpen(user_id, 0, 0, 0));
    LOG_INFO("userid: {}, pad handle: {:x}", user_id, pad_handle);
    mice.Init(user_id);
}

App::~App() {
    LOG_INFO("App stopped.");
    sceSystemServiceLoadExec("EXIT", nullptr);
}

bool App::HandleControllerInput() {
    scePadReadState(pad_handle, &pdata);
    static std::map<OrbisPadButton, bool> btn_pressed{};
    auto is_button_pressed = [&, this](OrbisPadButton b) {
        if (!btn_pressed.contains(b)) {
            btn_pressed[b] = false;
        }
        if ((pdata.buttons & b) != 0) {
            if (!btn_pressed[b]) {
                btn_pressed[b] = true;
                return true;
            }
        } else {
            btn_pressed[b] = false;
        }
        return false;
    };
    if (is_button_pressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        return false;
    }
    return true;
}

void App::DrawDemo() {
    renderer.scene->DrawRectangle(100, 100, 200, 200, {0, 0, 0});
    renderer.scene->DrawRectangle(120, 120, 160, 160, {255, 128, 128});
    renderer.scene->DrawRectangle(140, 140, 120, 120, {0, 0, 0});
    if (!use_font) {
        return;
    }
    renderer.scene->DrawText("Hello, Screen!", renderer.font, 400, 220, {50, 50, 50}, {0, 0, 255});
}