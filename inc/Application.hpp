#pragma once

#include <cstdint>
#include <memory>

#include "Scene.hpp"

namespace cosmic {

constexpr uint32_t SCREEN_WIDTH = 1920;
constexpr uint32_t SCREEN_HEIGHT = 1080;
constexpr uint32_t TARGET_FPS = 60;
constexpr const char* WINDOW_TITLE = "Cosmic Horizons";

class Application {
   private:
    std::unique_ptr<Scene> m_active_scene = nullptr;
    std::unique_ptr<Scene> m_next_scene = nullptr;

    bool m_running = true;

   public:
    bool Initialize() noexcept;
    void Run() noexcept;
    void Destroy() noexcept;

    template <typename T, typename... Args>
    void TransitionScene(Args&&... args) noexcept {
        m_next_scene = std::make_unique<T>(std::forward<Args>(args)...);
    }

    void Shutdown() noexcept {
        m_running = false;
    }

   private:
    void Update() noexcept;
    void Render() noexcept;
    void ProcessSceneTransition() noexcept;
};

}  // namespace cosmic