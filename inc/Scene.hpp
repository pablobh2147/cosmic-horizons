#pragma once

namespace cosmic {

class Application;

class Scene {
   public:
    virtual ~Scene() noexcept = default;

    virtual void Initialize() noexcept = 0;
    virtual void Update() noexcept = 0;
    virtual void Render() noexcept = 0;
    virtual void Shutdown() noexcept = 0;

    void SetApplication(Application* app) { m_app = app; }
    Application* GetApplication() const { return m_app; }

   private:
    Application* m_app = nullptr;
};

}  // namespace cosmic