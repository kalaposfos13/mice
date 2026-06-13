#include "scene_manager.h"

#include "app_context.h"
#include "scene.h"
#include "ui.h"

SceneManager::SceneManager(AppContext& ctx) : ctx(ctx) {}

void SceneManager::Push(std::unique_ptr<Scene> scene) {
    pending.push_back({
        .type = CommandType::Push,
        .scene = std::move(scene),
    });
}

void SceneManager::Pop() {
    pending.push_back({
        .type = CommandType::Pop,
    });
}

void SceneManager::Replace(std::unique_ptr<Scene> scene) {
    pending.push_back({
        .type = CommandType::Replace,
        .scene = std::move(scene),
    });
}

Scene* SceneManager::Current() {
    return stack.empty() ? nullptr : stack.back().get();
}

void SceneManager::Update(double dt) {
    if (transition_phase != TransitionPhase::None) {
        UpdateTransition(dt);
        return;
    }

    if (auto* scene = Current()) {
        scene->Update(ctx, dt);
    }

    ApplyCommands();
}

void SceneManager::Draw() {
    if (transition_phase == TransitionPhase::None) {
        DrawCurrentScenes();

        if (ctx.draw_mice) {
            UI::DrawCursors(ctx);
        }

        return;
    }

    float t = transition_timer / active_transition->Duration();

    t = std::clamp(t, 0.0f, 1.0f);

    if (transition_phase == TransitionPhase::Out) {
        active_transition->DrawOut(ctx, old_capture, t);
    } else {
        active_transition->DrawIn(ctx, new_capture, t);
    }

    if (ctx.draw_mice) {
        UI::DrawCursors(ctx);
    }
}

void SceneManager::DrawCurrentScenes() {
    if (stack.empty()) {
        return;
    }

    size_t first_visible = stack.size() - 1;

    while (first_visible > 0 && stack[first_visible]->IsOverlay()) {
        --first_visible;
    }

    bool old_accepting = ctx.accepting_inputs;

    ctx.accepting_inputs = false;

    for (size_t i = first_visible; i < stack.size() - 1; i++) {
        stack[i]->Draw(ctx);
    }

    ctx.accepting_inputs = true;

    stack.back()->Draw(ctx);

    ctx.accepting_inputs = old_accepting;
}

void SceneManager::ApplyCommands() {
    for (auto& cmd : pending) {
        if (RequiresTransition(cmd) && !Current()->IsOverlay()) {
            BeginTransition(std::move(cmd));
            break;
        }

        ExecuteCommand(cmd);
    }

    pending.clear();
}

void SceneManager::ExecuteCommand(Command& cmd) {
    LOG_INFO("called");
    switch (cmd.type) {
    case CommandType::Push: {
        if (cmd.scene) {
            cmd.scene->Enter(ctx);
            stack.push_back(std::move(cmd.scene));
        }
        break;
    }

    case CommandType::Pop: {
        if (!stack.empty()) {
            stack.back()->Leave(ctx);
            stack.pop_back();
        }

        if (stack.empty()) {
            ctx.running = false;
        }

        break;
    }

    case CommandType::Replace: {
        if (!stack.empty()) {
            stack.back()->Leave(ctx);
            stack.pop_back();
        }

        if (cmd.scene) {
            cmd.scene->Enter(ctx);
            stack.push_back(std::move(cmd.scene));
        }

        break;
    }
    }
}

bool SceneManager::RequiresTransition(Command const& cmd) {
    if (stack.size() == 0) {
        return false;
    }
    Scene* scene = nullptr;

    switch (cmd.type) {
    case CommandType::Push:
    case CommandType::Replace:
        scene = cmd.scene.get();
        break;

    case CommandType::Pop:
        if (stack.size() <= 1)
            return false;

        scene = stack[stack.size() - 2].get();
        break;
    }

    auto ret = scene && !scene->IsOverlay();

    LOG_INFO("{}", ret);

    return ret;
}

void SceneManager::BeginTransition(Command cmd) {
    LOG_INFO("called");
    transition_command = std::move(cmd);

    active_transition = std::make_unique<FadeTransition>();

    ctx.renderer.CaptureFramebuffer(old_capture);

    transition_timer = 0.0f;
    transition_phase = TransitionPhase::Out;
}

void SceneManager::UpdateTransition(double dt) {
    transition_timer += dt;

    float t = transition_timer / active_transition->Duration();

    if (transition_phase == TransitionPhase::Out) {
        if (t >= 1.0f) {
            ExecuteCommand(*transition_command);

            CaptureCurrentScene();

            transition_phase = TransitionPhase::In;
            transition_timer = 0.0f;
        }
    } else if (transition_phase == TransitionPhase::In) {
        if (t >= 1.0f) {
            transition_phase = TransitionPhase::None;
            transition_timer = 0.0f;

            active_transition.reset();
            transition_command.reset();
        }
    }
}

void SceneManager::CaptureCurrentScene() {
    LOG_INFO("called");
    ctx.renderer.scene->FrameBufferClear();
    DrawCurrentScenes();
    ctx.renderer.CaptureFramebuffer(new_capture);
}