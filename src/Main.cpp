#include "MyGame.h"

// Minimal Entry Point
// Resource allocation and initialisation handled in engine constructor (MyGame class)
int main(int argc, char** argv) {
    MyGame* app = new MyGame();
    app->run();
    app->cleanup();
    delete app;

    return 0;
}