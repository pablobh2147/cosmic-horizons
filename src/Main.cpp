#include "Application.hpp"
#include "scenes/MainMenuScene.hpp"

int main() {
    cosmic::Application app;

    app.Initialize();
    app.TransitionScene<cosmic::MainMenuScene>();
    app.Run();
    app.Destroy();

    return 0;
}