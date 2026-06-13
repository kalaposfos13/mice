#pragma once

#include "graphics.h"
#include "image.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();

    void BeginFrame();
    void EndFrame();

    void CaptureFramebuffer(Image& dst);
    void DrawImage(Image const& img, int x, int y);
    void DrawImageDarkened(Image const& img, int x, int y, float factor);

    Scene2D* GetScene() {
        return scene;
    }

    bool use_font = true;
    Scene2D* scene{};
    FT_Face font{};
};