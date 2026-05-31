#include "Application.hpp"

#include <raylib.h>

namespace cosmic {

bool Application::Initialize() noexcept {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Horizons");
    SetTargetFPS(TARGET_FPS);

    InitAudioDevice();

    return true;
}

void Application::Run() noexcept {
    while (!WindowShouldClose()) {
        Update();
        Render();
    }
}

void Application::Shutdown() noexcept {
    if (m_active_scene != nullptr) {
        m_active_scene->Shutdown();
    }

    CloseAudioDevice();
    CloseWindow();
}

void Application::Update() noexcept {
    if (IsKeyDown(KEY_F11)) {
        ToggleFullscreen();
    }

    if (m_active_scene != nullptr) {
        m_active_scene->Update();
    }
}

void Application::Render() noexcept {
    BeginDrawing();

    if (m_active_scene != nullptr) {
        m_active_scene->Render();
    }

    EndDrawing();
}

}  // namespace cosmic