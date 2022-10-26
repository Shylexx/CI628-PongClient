#include "MyGame.h"

// Minimal Entry Point
// Resource allocation and initialisation handled in engine constructor (MyGame class)
int main(int argc, char** argv) {

    MyGame app;
    app.run();
    app.cleanup();

    return 0;
}


/*
Gameplan:
- Team based (2 teams of 2 or asymmetric 1v2)
- Lobby with some kind of game customisation (or team allocation)
*/


/*
TODO:
- Find new ways to transfer data than just String messages?
- Some method of abstraction for game objects (ECS, etc)
- Asset/Resource handler
- 
*/