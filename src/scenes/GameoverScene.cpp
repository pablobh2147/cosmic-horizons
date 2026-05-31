#include "scenes/GameoverScene.hpp"

#include <raylib.h>

#include <cstdint>

#include "Application.hpp"
#include "scenes/MainMenuScene.hpp"

namespace cosmic {

void GameoverScene::Initialize() noexcept {
}

void GameoverScene::Update() noexcept {
    if (IsKeyPressed(KEY_SPACE)) {
        GetApplication()->TransitionScene<MainMenuScene>();
    }
}

void GameoverScene::Render() noexcept {
    ClearBackground(BLACK);

    constexpr uint32_t TEXT_SIZE = 50;
    constexpr Color TEXT_COLOR = RED;

    const char* message = "GAME OVER";
    int text_width = MeasureText(message, TEXT_SIZE);
    DrawText(message, (GetScreenWidth() - text_width) / 2, (GetScreenHeight() - TEXT_SIZE) / 2, TEXT_SIZE, TEXT_COLOR);

    constexpr uint32_t CONTINUE_TEXT_SIZE = 20;
    const char* continue_message = "Press Space to continue";
    int continue_text_width = MeasureText(continue_message, CONTINUE_TEXT_SIZE);
    DrawText(continue_message, (GetScreenWidth() - continue_text_width) / 2, (GetScreenHeight() - CONTINUE_TEXT_SIZE) / 2 + TEXT_SIZE + 20, CONTINUE_TEXT_SIZE, LIGHTGRAY);
}

void GameoverScene::Shutdown() noexcept {
}

}  // namespace cosmic