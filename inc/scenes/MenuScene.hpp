#pragma once

#include "Scene.hpp"

namespace cosmic {

class MenuScene : public Scene {
   public:
    void Initialize() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;
    void Shutdown() noexcept override;
};

}  // namespace cosmic