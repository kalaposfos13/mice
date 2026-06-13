#include <filesystem>
#include "app.h"

std::string APP_ROOT = "/app0/";

int main(void) {
    if (!std::filesystem::exists("/app0/.is-app0-root")) {
        APP_ROOT = "/data/homebrew/";
    }
    App app{};
    app.Run();
}
