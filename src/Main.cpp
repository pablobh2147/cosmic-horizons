#include "Application.hpp"
#include "scenes/GameScene.hpp"
#include "scenes/MenuScene.hpp"

int main() {
    cosmic::Application app;

    app.Initialize();
    app.SetActiveScene<cosmic::GameScene>();
    app.Run();
    app.Shutdown();

    return 0;
}