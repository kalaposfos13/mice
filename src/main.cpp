#include <filesystem>
#include "app.h"

namespace fs = std::filesystem;

fs::path APP_ROOT = "/app0/";
fs::path WORKDIR_ROOT = "/data/homebrew/mice/";

int main(void) {
    if (!fs::exists("/app0/.is-app0-root")) {
        APP_ROOT = "/data/homebrew/";
        if (!fs::exists(APP_ROOT)) {
            fs::create_directories(APP_ROOT);
        }
    }
    if (!fs::exists(WORKDIR_ROOT)) {
        fs::create_directories(WORKDIR_ROOT);
    }
    App app{};
    app.Run();
}
