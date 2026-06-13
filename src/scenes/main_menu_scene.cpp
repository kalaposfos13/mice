#include "app_context.h"
#include "scene.h"
#include "ui.h"

#define THIS_SCENE MainMenuScene

static double deltatime = 0.5;
static std::invoke_result_t<decltype(std::chrono::steady_clock::now)> steady_time{};
static double steady_dt = 0;

void THIS_SCENE::Enter(AppContext& ctx) {
    LOG_INFO("enter");
    steady_time = std::chrono::steady_clock::now();
    ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2 + 300);
    ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2 + 300);
}

void THIS_SCENE::Leave(AppContext& ctx) {}

void THIS_SCENE::Update(AppContext& ctx, double dt) {
    deltatime = dt;
    auto steady_now = std::chrono::steady_clock::now();
    steady_dt = std::chrono::duration<double>(steady_now - steady_time).count();
    steady_time = steady_now;

    if (ctx.pad.IsPressed(OrbisPadButton::ORBIS_PAD_BUTTON_CIRCLE)) {
        ctx.scenes.Pop();
    }
    if ((ctx.mice[0].clicked_buttons.Includes(MouseButton::Middle)) != 0) {
        ctx.mice.SetCursor(0, 1920 / 2, 1080 / 2 + 300);
    }
    if ((ctx.mice[1].clicked_buttons.Includes(MouseButton::Middle)) != 0) {
        ctx.mice.SetCursor(1, 1920 / 2, 1080 / 2 + 300);
    }
}

void THIS_SCENE::Draw(AppContext& ctx) {
    UI ui{ctx};

    static bool checkbox_value = false;
    static float slider_value = 5.0f, slider_r_value = 5.0f;
    static float progress = 0.0f;
    static float steady_progress = 0.0f;

    progress += deltatime;
    if (progress > 1.0f)
        progress = 0.0f;

    steady_progress += steady_dt;
    if (steady_progress > 1.0f)
        steady_progress = 0.0f;

    VerticalLayout layout{
        .x = 80,
        .y = 80,
        .width = 500,
        .height = 60,
        .spacing = 15,
    };

    ui.Panel({50, 50, 575, 750});

    ui.Label({1000, 60}, "UI DEBUG SCENE", FontSize::Large);

    // Buttons
    if (ui.Button(layout.Next(), "BUTTON A").released) {
        ui.Label({650, 100}, "BUTTON A released", FontSize::Small);
    }

    if (ui.Button(layout.Next(), "BUTTON B").clicked) {
        ui.Label({650, 130}, "BUTTON B clicked", FontSize::Small);
    }

    if (ui.Button(layout.Next(), "BUTTON C").held) {
        ui.Label({650, 160}, "BUTTON C held", FontSize::Small);
    }

    ui.Separator(layout.Next());

    // Checkbox
    auto cb = ui.Checkbox(layout.Next(), checkbox_value, "");
    checkbox_value = cb.value;

    ui.Label({650, 400}, cb.value ? "Checkbox: ON" : "Checkbox: OFF", FontSize::Small);

    // Slider
    auto sl = ui.Slider(layout.Next(), slider_value, 0.0f, 10.0f);
    slider_value = sl.value;

    ui.Label({650, 470}, "Slider: " + std::to_string((int)slider_value), FontSize::Small);

    auto sl_r = ui.Slider(layout.Next(), slider_r_value, 0.0f, 10.0f, 1);
    slider_r_value = sl_r.value;

    ui.Label({650, 540}, "Slider (ratcheted): " + std::to_string((int)slider_r_value),
             FontSize::Small);

    ui.ProgressBar(layout.Next(), progress);

    ui.Label({650, 610}, "system_clock: " + std::to_string((int)(progress * 100)) + "%",
             FontSize::Small);

    ui.ProgressBar(layout.Next(), steady_progress);

    ui.Label({650, 680}, "steady_clock: " + std::to_string((int)(steady_progress * 100)) + "%",
             FontSize::Small);

    // Multi-mouse test area
    auto big = ui.Button({1050, 420, 400, 120}, "BUTTON D", FontSize::Large);

    if (big.held_m0) {
        ui.Label({1100, 550}, "Mouse 0 holding", FontSize::Small);
    }
    if (big.held_m1) {
        ui.Label({1100, 580}, "Mouse 1 holding", FontSize::Small);
    }

    if (big.hovered_m0 && big.hovered_m1) {
        ui.Label({1100, 610}, "Both mice hovering", FontSize::Small);
    }
}
