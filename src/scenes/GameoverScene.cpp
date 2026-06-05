#include "scenes/GameoverScene.hpp"

#include <raylib.h>

#include <cstdint>
#include <format>

#include "Application.hpp"
#include "Common.hpp"
#include "scenes/MainMenuScene.hpp"

namespace cosmic {

GameoverScene::GameoverScene(const Statistics& stats) noexcept : m_statistics(stats) {}

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
    constexpr uint32_t STATS_TITLE_SIZE = 30;
    constexpr uint32_t STATS_TEXT_SIZE = 20;
    constexpr uint32_t CONTINUE_TEXT_SIZE = 20;
    constexpr uint32_t LINE_SPACING = 30;
    constexpr uint32_t STATS_SPACING = 20;

    const int32_t center_x = GetScreenWidth() / 2;
    int32_t current_y = GetScreenHeight() / 2 - 150;

    DrawTextCentered("GAME OVER", center_x, current_y, GAMEOVER_TEXT_SIZE, RED);
    current_y += GAMEOVER_TEXT_SIZE + LINE_SPACING;

    DrawTextCentered("Final Statistics", center_x, current_y, STATS_TITLE_SIZE, YELLOW);
    current_y += STATS_TITLE_SIZE + LINE_SPACING;

    DrawTextCentered(std::format("Level Reached: {}", m_statistics.level).c_str(), center_x, current_y, STATS_TEXT_SIZE, WHITE);
    current_y += STATS_TEXT_SIZE + LINE_SPACING;

    DrawTextCentered(std::format("Final Score: {}", m_statistics.score).c_str(), center_x, current_y, STATS_TEXT_SIZE, WHITE);
    current_y += STATS_TEXT_SIZE + LINE_SPACING;

    DrawTextCentered(std::format("Asteroids Destroyed: {}", m_statistics.asteroids_destroyed).c_str(), center_x, current_y, STATS_TEXT_SIZE, WHITE);
    current_y += STATS_TEXT_SIZE + LINE_SPACING;

    DrawTextCentered(std::format("Shots Fired: {}", m_statistics.shots_fired).c_str(), center_x, current_y, STATS_TEXT_SIZE, WHITE);
    current_y += STATS_TEXT_SIZE + LINE_SPACING;

    const float accuracy = m_statistics.GetAccuracy() * 100.0F;
    DrawTextCentered(std::format("Accuracy: {:.1f}%", accuracy).c_str(), center_x, current_y, STATS_TEXT_SIZE, WHITE);
    current_y += STATS_TEXT_SIZE + LINE_SPACING + STATS_SPACING;

    DrawTextCentered("Press Space to continue", center_x, current_y, CONTINUE_TEXT_SIZE, LIGHTGRAY);
}

}  // namespace cosmic