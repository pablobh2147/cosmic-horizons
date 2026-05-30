#include "scenes/GameScene.hpp"

#include <raylib.h>

namespace cosmic {

void GameScene::Initialize() noexcept {
}

void GameScene::Update() noexcept {
}

void GameScene::Render() noexcept {
    ClearBackground(BLACK);
    DrawText("Game Scene", 100, 100, 20, WHITE);
}

void GameScene::Shutdown() noexcept {
}

}  // namespace cosmic
