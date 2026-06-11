#include "common/assert.h"
#include "renderer.h"

std::string APP_ROOT;

Renderer::Renderer() {
    Init();
}

Renderer::~Renderer() {
    FT_Done_Face(font);
}

void Renderer::Init() {
    if (!scene) {
        scene = new Scene2D(1920, 1080, 4);
        ASSERT_MSG(scene->Init(0xC000000, 2), "Failed to initialize 2D scene");
    }
    if (use_font && scene->ftLib) {
        ASSERT_OK(scene->InitFontLib());
        std::string font_path = APP_ROOT + "assets/fonts/Monocraft.ttf";
        if (scene->InitFont(&font, font_path.c_str(), 80) && font != nullptr) {
            LOG_ERROR("Failed to init font");
            use_font = false;
        }
    }
}

void Renderer::BeginFrame() {
    scene->FrameBufferClear();
}

void Renderer::EndFrame() {
    scene->SubmitFlip();
    scene->FrameWait();
    scene->FrameBufferSwap();
}

void Renderer::DrawImage(const Image& img, int x, int y) {
    auto* fb = (uint32_t*)scene->frameBuffers[scene->activeFrameBufferIdx];
    ASSERT(fb != nullptr);
    ASSERT(img.pixels != nullptr);

    for (int j = 0; j < img.height; j++) {
        for (int i = 0; i < img.width; i++) {
            int sx = x + i;
            int sy = y + j;

            if (sx < 0 || sy < 0 || sx >= scene->width || sy >= scene->height)
                continue;

            fb[sy * scene->width + sx] = img.pixels[j * img.width + i];
        }
    }
}
