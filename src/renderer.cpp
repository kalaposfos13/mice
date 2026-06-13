#include "common/assert.h"
#include "renderer.h"

extern std::string APP_ROOT;

Renderer::Renderer() {
    Init();
}

Renderer::~Renderer() {
    if (use_font)
        FT_Done_Face(font);
}

void Renderer::Init() {
    if (!scene) {
        scene = new Scene2D(1920, 1080, 4);
        ASSERT_MSG(scene->Init(0xC000000, 2), "Failed to initialize 2D scene");
    }
    if (use_font && !scene->ftLib) {
        ASSERT_OK(scene->InitFontLib());
        std::string font_path = APP_ROOT + "assets/fonts/Monocraft.ttf";
        if (!scene->InitFont(&font, font_path.c_str(), 30) || font == nullptr) {
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

void Renderer::CaptureFramebuffer(Image& dst) {
    LOG_INFO("called");
    u32* fb = reinterpret_cast<u32*>(scene->frameBuffers[scene->activeFrameBufferIdx]);

    ASSERT(fb != nullptr);

    const int width = scene->width;
    const int height = scene->height;

    if (dst.width != width || dst.height != height || dst.pixels == nullptr) {
        dst.Free();
        ASSERT(dst.Allocate(width, height));
    }

    std::memcpy(dst.pixels, fb, width * height * sizeof(uint32_t));
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

void Renderer::DrawImageDarkened(const Image& img, int x, int y, float factor) {
    factor = std::clamp(factor, 0.0f, 1.0f);

    auto* fb = reinterpret_cast<uint32_t*>(scene->frameBuffers[scene->activeFrameBufferIdx]);

    ASSERT(fb != nullptr);
    ASSERT(img.pixels != nullptr);

    for (int j = 0; j < img.height; j++) {
        for (int i = 0; i < img.width; i++) {
            int sx = x + i;
            int sy = y + j;

            if (sx < 0 || sy < 0 || sx >= scene->width || sy >= scene->height) {
                continue;
            }

            uint32_t c = img.pixels[j * img.width + i];

            uint8_t r = (c >> 16) & 0xff;
            uint8_t g = (c >> 8) & 0xff;
            uint8_t b = c & 0xff;

            r = static_cast<uint8_t>(r * factor);
            g = static_cast<uint8_t>(g * factor);
            b = static_cast<uint8_t>(b * factor);

            fb[sy * scene->width + sx] = (r << 16) | (g << 8) | b;
        }
    }
}
