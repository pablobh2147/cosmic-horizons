#include "scenes/GameoverScene.hpp"

#include <raylib.h>

#include <cstdint>

#include "Application.hpp"
#include "Common.hpp"
#include "scenes/MainMenuScene.hpp"

namespace cosmic {

void GameoverScene::Initialize() noexcept {}

void GameoverScene::Shutdown() noexcept {}

void GameoverScene::Update() noexcept {
    if (IsKeyPressed(KEY_SPACE)) {
        GetApplication()->TransitionScene<MainMenuScene>();
    }
}

void GameoverScene::Render() noexcept {
    ClearBackground(BLACK);

    constexpr uint32_t GAMEOVER_TEXT_SIZE = 50;
    constexpr uint32_t CONTINUE_TEXT_SIZE = 20;
    constexpr uint32_t VERTICAL_SPACING = 20;

    DrawTextCentered("GAME OVER", 0, 0, GAMEOVER_TEXT_SIZE, RED);
    DrawTextCentered("Press Space to continue", 0, GAMEOVER_TEXT_SIZE + VERTICAL_SPACING, CONTINUE_TEXT_SIZE, LIGHTGRAY);
}

}  // namespace cosmic