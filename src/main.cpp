#include "app.h"
#include <filesystem>

std::string APP_ROOT = "/app0/";

int main(void) {
    if (!std::filesystem::exists("/app0/.is-pkg-build")) {
        APP_ROOT = "/data/homebrew/";
    }
    App app{};
    app.Run();
}
