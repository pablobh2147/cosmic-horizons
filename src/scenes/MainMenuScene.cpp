#include "scenes/MainMenuScene.hpp"

#include <raylib.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "Application.hpp"
#include "scenes/GameScene.hpp"

namespace cosmic {

void MainMenuScene::Initialize() noexcept {
    m_selected_option = 0;
    m_animation_time = 0.0F;
    m_rotation_angle = 0.0F;
    m_starfield.Generate(800);
}

void MainMenuScene::Update() noexcept {
    m_animation_time += GetFrameTime();
    m_rotation_angle += GetFrameTime() * 0.1F;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        m_selected_option--;
        if (m_selected_option < 0) {
            m_selected_option = MENU_OPTION_COUNT - 1;
        }
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        m_selected_option++;
        if (m_selected_option >= MENU_OPTION_COUNT) {
            m_selected_option = 0;
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (m_selected_option) {
            case 0:
                GetApplication()->TransitionScene<GameScene>();
                break;
            case 1:
                GetApplication()->Shutdown();
                break;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        GetApplication()->Shutdown();
    }
}

void MainMenuScene::Render() noexcept {
    ClearBackground(BLACK);

    constexpr float FOV = 70.0F;
    constexpr float NEAR_PLANE = 0.1F;
    constexpr float FAR_PLANE = 100.0F;

    float aspect = static_cast<float>(GetScreenWidth()) / static_cast<float>(GetScreenHeight());
    glm::mat4 projection = glm::perspective(glm::radians(FOV), aspect, NEAR_PLANE, FAR_PLANE);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0F), m_rotation_angle, glm::vec3(0.2F, 1.0F, 0.1F));
    m_starfield.Render(projection * rotation, m_animation_time);

    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    constexpr int TITLE_SIZE = 80;
    const char* title = "COSMIC HORIZONS";
    int title_width = MeasureText(title, TITLE_SIZE);
    DrawText(title, (screen_w - title_width) / 2, screen_h / 5, TITLE_SIZE, WHITE);

    constexpr int SUBTITLE_SIZE = 20;
    const char* subtitle = "Navigate the stars";
    int subtitle_width = MeasureText(subtitle, SUBTITLE_SIZE);
    DrawText(subtitle, (screen_w - subtitle_width) / 2, screen_h / 5 + TITLE_SIZE + 10, SUBTITLE_SIZE, LIGHTGRAY);

    constexpr int OPTION_SIZE = 40;
    constexpr int OPTION_SPACING = 60;
    const char* options[] = {"Play", "Quit"};

    int menu_start_y = screen_h / 2;

    for (int i = 0; i < MENU_OPTION_COUNT; i++) {
        Color color = (i == m_selected_option) ? YELLOW : GRAY;
        int option_width = MeasureText(options[i], OPTION_SIZE);
        int x = (screen_w - option_width) / 2;
        int y = menu_start_y + i * OPTION_SPACING;

        DrawText(options[i], x, y, OPTION_SIZE, color);

        if (i == m_selected_option) {
            DrawText(">", x - 40, y, OPTION_SIZE, YELLOW);
        }
    }

    constexpr int HINT_SIZE = 16;
    const char* hint = "[W/S or UP/DOWN] Navigate  |  [ENTER] Select  |  [ESC] Quit";
    int hint_width = MeasureText(hint, HINT_SIZE);
    DrawText(hint, (screen_w - hint_width) / 2, screen_h - 50, HINT_SIZE, DARKGRAY);
}

void MainMenuScene::Shutdown() noexcept {
}

}  // namespace cosmic