#pragma once

#include <cstdint>
#include <memory>

#include "Scene.hpp"

namespace cosmic {

constexpr uint32_t SCREEN_WIDTH = 1920;
constexpr uint32_t SCREEN_HEIGHT = 1080;
constexpr uint32_t TARGET_FPS = 60;

class Application {
   private:
    std::unique_ptr<Scene> m_active_scene = nullptr;

   public:
    bool Initialize() noexcept;
    void Run() noexcept;
    void Shutdown() noexcept;

    template <typename T, typename... Args>
    void SetActiveScene(Args&&... args) noexcept {
        if (m_active_scene != nullptr) {
            m_active_scene->Shutdown();
        }

        m_active_scene = std::make_unique<T>(std::forward<Args>(args)...);
        m_active_scene->Initialize();
    }

   private:
    void Update() noexcept;
    void Render() noexcept;
};

}  // namespace cosmic