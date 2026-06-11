#pragma once

class AppContext;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void Enter(AppContext& ctx) {}
    virtual void Leave(AppContext& ctx) {}

    virtual void Update(AppContext& ctx, float dt) = 0;
    virtual void Draw(AppContext& ctx) = 0;
};

class BasicScene : public Scene {
public:
    void Enter(AppContext& ctx) override;
    void Leave(AppContext& ctx) override;
    void Update(AppContext& ctx, float dt) override;
    void Draw(AppContext& ctx) override;
};