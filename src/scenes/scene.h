#pragma once

class AppContext;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void Enter(AppContext& ctx) {}
    virtual void Leave(AppContext& ctx) {}

    virtual void Update(AppContext& ctx, float dt) = 0;
    virtual void Draw(AppContext& ctx) = 0;

    virtual bool BlocksScenesBelow() const {
        return true;
    }
};

#define DECLARE_SCENE(name)                                                                        \
    class name : public Scene {                                                                    \
    public:                                                                                        \
        void Enter(AppContext& ctx) override;                                                      \
        void Leave(AppContext& ctx) override;                                                      \
        void Update(AppContext& ctx, float dt) override;                                           \
        void Draw(AppContext& ctx) override;                                                       \
    };
#define DECLARE_OVERLAY_SCENE(name)                                                                \
    class name : public Scene {                                                                    \
    public:                                                                                        \
        void Enter(AppContext& ctx) override;                                                      \
        void Leave(AppContext& ctx) override;                                                      \
        void Update(AppContext& ctx, float dt) override;                                           \
        void Draw(AppContext& ctx) override;                                                       \
        bool BlocksScenesBelow() const override {                                                  \
            return false;                                                                          \
        }                                                                                          \
    };

DECLARE_SCENE(EmptyScene)
DECLARE_SCENE(BasicScene)
DECLARE_SCENE(MainMenuScene)
DECLARE_SCENE(SettingsScene)
DECLARE_SCENE(VideoSettingsScene)
DECLARE_SCENE(AudioSettingsScene)
DECLARE_OVERLAY_SCENE(AboutOverlayScene)