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

void SceneManager::Update(float dt) {
    if (auto* scene = Current()) {
        scene->Update(ctx, dt);
    }

    ApplyCommands();
}

void SceneManager::Draw() {
    if (stack.empty()) {
        return;
    }

    size_t first_visible = stack.size() - 1;

    while (first_visible > 0 && !stack[first_visible]->BlocksScenesBelow()) {
        --first_visible;
    }
    ctx.accepting_inputs = false;
    for (size_t i = first_visible; i < stack.size() - 1; i++) {
        stack[i]->Draw(ctx);
    }
    ctx.accepting_inputs = true;
    stack[stack.size() - 1]->Draw(ctx);
    if (ctx.draw_mice) {
        UI::DrawCursors(ctx);
    }
}

void SceneManager::ApplyCommands() {
    for (auto& cmd : pending) {
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

    pending.clear();
}