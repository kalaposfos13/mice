#pragma once

#include "ui/ui.h"
#include "ui/widget_manager.h"

class AppContext;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void Enter() {}
    virtual void Leave() {}

    virtual void Update(double dt) = 0;
    virtual void Draw() = 0;

    virtual bool IsOverlay() const {
        return false;
    }
};

#define DECLARE_SCENE(name)                                                                        \
    class name : public Scene {                                                                    \
    public:                                                                                        \
        void Enter() override;                                                                     \
        void Leave() override;                                                                     \
        void Update(double dt) override;                                                           \
        void Draw() override;                                                                      \
    };
#define DECLARE_OVERLAY_SCENE(name)                                                                \
    class name final : public Scene {                                                              \
    public:                                                                                        \
        void Enter() override;                                                                     \
        void Leave() override;                                                                     \
        void Update(double dt) override;                                                           \
        void Draw() override;                                                                      \
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
    void Enter() override;
    void Leave() override;
    void Update(double dt) override;
    void Draw() override;

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

    void Enter() override;
    void Leave() override;
    void Update(double dt) override;
    void Draw() override;

private:
    std::filesystem::path panel_path_;

    WidgetManager wm_;
};