#pragma once

#include <memory>
#include <vector>

class AppContext;
class Scene;

class SceneManager {
public:
    enum class CommandType {
        Push,
        Pop,
        Replace,
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

    void Update(float dt);
    void Draw();

private:
    void ApplyCommands();

    AppContext& ctx;

    std::vector<std::unique_ptr<Scene>> stack;
    std::vector<Command> pending;
};