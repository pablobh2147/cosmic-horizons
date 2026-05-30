#include "Application.hpp"

#include <raylib.h>

namespace cosmic {

bool Application::Initialize() noexcept {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Horizons");
    SetTargetFPS(TARGET_FPS);

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

    CloseWindow();
}

void Application::Update() noexcept {
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