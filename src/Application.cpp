#include "Application.hpp"

#include <raylib.h>

namespace cosmic {

bool Application::Initialize() noexcept {
    m_running = true;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    // SetExitKey(KEY_NULL);

    InitAudioDevice();

    return true;
}

void Application::Run() noexcept {
    while (!WindowShouldClose() && m_running) {
        ProcessSceneTransition();
        Update();
        Render();
    }
}

void Application::Destroy() noexcept {
    m_running = false;

    if (m_active_scene != nullptr) {
        m_active_scene->Shutdown();
        m_active_scene = nullptr;
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

void Application::ProcessSceneTransition() noexcept {
    if (m_next_scene != nullptr) {
        if (m_active_scene != nullptr) {
            m_active_scene->Shutdown();
        }

        m_active_scene = std::move(m_next_scene);
        m_active_scene->SetApplication(this);
        m_active_scene->Initialize();
    }
}

}  // namespace cosmic