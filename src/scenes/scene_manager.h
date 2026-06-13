#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "transitions/transition.h"

class AppContext;
class Scene;

class SceneManager {
public:
    enum class CommandType {
        Push,
        Pop,
        Replace,
    };
    enum class TransitionPhase {
        None,
        Out,
        In,
    };

    struct Command {
        CommandType type;
        std::unique_ptr<Scene> scene;
    };

    explicit SceneManager(AppContext& ctx);

    void Push(std::unique_ptr<Scene> scene);
    void Pop();
    void Replace(std::unique_ptr<Scene> scene);

    template <typename T, typename... Args>
    void Push(Args&&... args) {
        Push(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <typename T, typename... Args>
    void Replace(Args&&... args) {
        Replace(std::make_unique<T>(std::forward<Args>(args)...));
    }

    Scene* Current();

    void Update(double dt);
    void Draw();
    void DrawCurrentScenes();

private:
    void ApplyCommands();
    void ExecuteCommand(Command& cmd);
    bool RequiresTransition(Command const& cmd);
    void BeginTransition(Command cmd);
    void UpdateTransition(double dt);
    void CaptureCurrentScene();
    AppContext& ctx;

    std::vector<std::unique_ptr<Scene>> stack;
    std::vector<Command> pending;

    TransitionPhase transition_phase = TransitionPhase::None;

    float transition_timer = 0.0f;

    std::unique_ptr<Transition> active_transition;

    std::optional<Command> transition_command;

    Image old_capture;
    Image new_capture;
};