#pragma once

namespace cosmic {

class Scene {
   public:
    virtual ~Scene() noexcept = default;

    virtual void Initialize() noexcept = 0;
    virtual void Update() noexcept = 0;
    virtual void Render() noexcept = 0;
    virtual void Shutdown() noexcept = 0;
};

}  // namespace cosmic