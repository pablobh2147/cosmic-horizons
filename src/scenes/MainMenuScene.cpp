#include "scenes/MainMenuScene.hpp"

#include <raylib.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "Application.hpp"
#include "Common.hpp"
#include "scenes/GameScene.hpp"

namespace cosmic {

// ----------------------- Scene Methods -----------------------

void MainMenuScene::Initialize() noexcept {
    constexpr uint32_t STARFIELD_COUNT = 800;

    m_selected_option = 0;
    m_animation_time = 0.0F;
    m_rotation_angle = 0.0F;
    m_starfield.Generate(STARFIELD_COUNT);
}

void MainMenuScene::Shutdown() noexcept {}

void MainMenuScene::Update() noexcept {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        MoveSelectionUp();
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        MoveSelectionDown();
    }

    if (IsKeyPressed(KEY_ENTER)) {
        ExecuteSelection();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        GetApplication()->Shutdown();
    }
}

void MainMenuScene::Render() noexcept {
    ClearBackground(BLACK);

    m_animation_time += GetFrameTime();

    RenderBackground();
    DrawTitle();
    DrawMenu();
    DrawHint();
}

// ----------------------- Drawing & Rendering Methods -----------------------

void MainMenuScene::RenderBackground() noexcept {
    constexpr float FOV = 70.0F;
    constexpr float NEAR_PLANE = 0.1F;
    constexpr float FAR_PLANE = 100.0F;

    m_rotation_angle += GetFrameTime() * 0.1F;

    float aspect = static_cast<float>(GetScreenWidth()) / static_cast<float>(GetScreenHeight());
    glm::mat4 projection = glm::perspective(glm::radians(FOV), aspect, NEAR_PLANE, FAR_PLANE);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0F), m_rotation_angle, glm::vec3(0.2F, 1.0F, 0.1F));

    m_starfield.Render(projection * rotation, m_animation_time);
}

void MainMenuScene::DrawTitle() noexcept {
    constexpr int32_t TITLE_SIZE = 80;
    constexpr int32_t SUBTITLE_SIZE = 20;
    constexpr int32_t VERTICAL_SPACING = 10;

    DrawTextCentered("COSMIC HORIZONS", GetScreenWidth() / 2, GetScreenHeight() / 5, TITLE_SIZE, WHITE);
    DrawTextCentered("Navigate the stars", GetScreenWidth() / 2, GetScreenHeight() / 5 + TITLE_SIZE + VERTICAL_SPACING, SUBTITLE_SIZE, LIGHTGRAY);
}

void MainMenuScene::DrawMenu() noexcept {
    constexpr int32_t OPTION_SIZE = 40;
    constexpr int32_t OPTION_SPACING = 60;
    constexpr Color SELECTED_COLOR = YELLOW;
    constexpr Color UNSELECTED_COLOR = GRAY;
    constexpr int32_t ARROW_OFFSET = 40;

    int32_t menu_start_y = GetScreenHeight() / 2;

    for (int32_t i = 0; i < static_cast<int32_t>(MENU_OPTIONS.size()); i++) {
        Color color = (i == m_selected_option) ? SELECTED_COLOR : UNSELECTED_COLOR;

        int option_width = MeasureText(MENU_OPTIONS[i], OPTION_SIZE);
        int x = (GetScreenWidth() - option_width) / 2;
        int y = menu_start_y + i * OPTION_SPACING;

        DrawText(MENU_OPTIONS[i], x, y, OPTION_SIZE, color);

        if (i == m_selected_option) {
            DrawText(">", x - ARROW_OFFSET, y, OPTION_SIZE, SELECTED_COLOR);
        }
    }
}

void MainMenuScene::DrawHint() noexcept {
    constexpr int32_t TEXT_SIZE = 16;
    constexpr int32_t TEXT_OFFSET = 50;

    DrawTextCentered(
        "[W/S or UP/DOWN] Navigate  |  [ENTER] Select  |  [ESC] Quit",
        GetScreenWidth() / 2,
        GetScreenHeight() - TEXT_OFFSET,
        TEXT_SIZE,
        DARKGRAY  //
    );
}

// ----------------------- Menu Navigation Methods -----------------------

void MainMenuScene::MoveSelectionUp() noexcept {
    m_selected_option--;
    if (m_selected_option < 0) {
        m_selected_option = static_cast<int32_t>(MENU_OPTIONS.size()) - 1;
    }
}

void MainMenuScene::MoveSelectionDown() noexcept {
    m_selected_option++;
    if (m_selected_option >= static_cast<int32_t>(MENU_OPTIONS.size())) {
        m_selected_option = 0;
    }
}

void MainMenuScene::ExecuteSelection() noexcept {
    switch (m_selected_option) {
        case 0:
            GetApplication()->TransitionScene<GameScene>();
            break;
        case 1:
            GetApplication()->Shutdown();
            break;
    }
}

}  // namespace cosmic