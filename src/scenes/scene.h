#pragma once

#include "ui/ui.h"
#include "ui/widget_manager.h"

class AppContext;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void Enter(AppContext& ctx) {}
    virtual void Leave(AppContext& ctx) {}

    virtual void Update(AppContext& ctx, double dt) = 0;
    virtual void Draw(AppContext& ctx) = 0;

    virtual bool IsOverlay() const {
        return false;
    }
};

#define DECLARE_SCENE(name)                                                                        \
    class name : public Scene {                                                                    \
    public:                                                                                        \
        void Enter(AppContext& ctx) override;                                                      \
        void Leave(AppContext& ctx) override;                                                      \
        void Update(AppContext& ctx, double dt) override;                                          \
        void Draw(AppContext& ctx) override;                                                       \
    };
#define DECLARE_OVERLAY_SCENE(name)                                                                \
    class name final : public Scene {                                                              \
    public:                                                                                        \
        void Enter(AppContext& ctx) override;                                                      \
        void Leave(AppContext& ctx) override;                                                      \
        void Update(AppContext& ctx, double dt) override;                                          \
        void Draw(AppContext& ctx) override;                                                       \
        bool IsOverlay() const override {                                                          \
            return true;                                                                           \
        }                                                                                          \
    };

DECLARE_SCENE(EmptyScene)
DECLARE_SCENE(BasicScene)
DECLARE_SCENE(MainMenuScene)
DECLARE_SCENE(SettingsScene)
DECLARE_SCENE(VideoSettingsScene)
DECLARE_SCENE(AudioSettingsScene)
DECLARE_OVERLAY_SCENE(AboutOverlayScene)
DECLARE_SCENE(SceneDesignerScene)

class PanelBrowserScene final : public Scene {
    void Enter(AppContext& ctx) override;
    void Leave(AppContext& ctx) override;
    void Update(AppContext& ctx, double dt) override;
    void Draw(AppContext& ctx) override;

private:
    struct PanelEntry {
        std::filesystem::path path;
        std::string display_name;
    };
    std::vector<PanelEntry> panels_;
};

class PanelViewerScene final : public Scene {
public:
    explicit PanelViewerScene(std::filesystem::path panel_path);

    void Enter(AppContext& ctx) override;
    void Leave(AppContext& ctx) override;
    void Update(AppContext& ctx, double dt) override;
    void Draw(AppContext& ctx) override;

private:
    std::filesystem::path panel_path_;

    WidgetManager wm_;
};