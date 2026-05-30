#include "scenes/MenuScene.hpp"

#include <raylib.h>

namespace cosmic {

void MenuScene::Initialize() noexcept {
}

void MenuScene::Update() noexcept {
}

void MenuScene::Render() noexcept {
    ClearBackground(BLACK);

    DrawText("Menu Scene", 100, 100, 20, WHITE);
}

void MenuScene::Shutdown() noexcept {
}

}  // namespace cosmic