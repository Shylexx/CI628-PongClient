#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_net.h"
#include "MyGame.h"
#include "Gameplay/Physics.h"

// Constructor / Init
MyGame::MyGame() {

    // Initialize SDL
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    IPaddress ip;

    // Resolve host (ip name + port) into an IPaddress type
    if (SDLNet_ResolveHost(&ip, IP_NAME, PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(3);
    }

    // Open the connection to the server
    m_Socket = SDLNet_TCP_Open(&ip);

    if (!m_Socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(4);
    }

    SDL_CreateThread(Net::on_receive, "ConnectionReceiveThread", (void*)this);
    SDL_CreateThread(Net::on_send, "ConnectionSendThread", (void*)this);

    if (!IMG_Init) {
        printf("SDL Image failed to load!");
        exit(4);
    }

    if (!TTF_Init) {
        printf("SDL TTF Failed to load!");
        exit(4);
    }


    
    // Engine Init

    m_Graphics->Init();

    m_Assets->Init(m_Graphics);

    m_Scene = new ECS::Scene();

    player = m_Scene->NewEntity();
    m_Scene->AddComponents(player, CompTags::Transform);
    m_Scene->m_Transforms[player].m_Rect = { 0, 0, 20, 60 };
}

int MyGame::run() {
  
    // ECS TESTING

    SDL_Log("ECS Test Start");

    // Empty Entity
    m_Scene->NewEntity();

    // Entity with a name component
    Entity testEntity = m_Scene->NewEntity();
    m_Scene->AddComponents(testEntity, CompTags::Name);
    m_Scene->m_Names[testEntity] = "Test Entity Name";

    // Empty Entity
    m_Scene->NewEntity();

    // Print names of all entities with name components
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        // Skip names of entities without names
        if (!m_Scene->HasComponents(e, CompTags::Name)) { continue; }

        std::cout << m_Scene->m_Names[e] << std::endl;
    }

    SDL_Log("ECS Test End");


    main_loop();

    return 0;
}

void MyGame::main_loop() {
    SDL_Event event;

    while (!m_ShouldQuit) {
        // Timekeeping
        m_Last = m_Now;
        m_Now = SDL_GetPerformanceCounter();
        deltaTime = ((m_Now - m_Last)*1000 / (double)SDL_GetPerformanceFrequency() );

        // input
        while (SDL_PollEvent(&event)) {
            if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
                input(event);

                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    m_ShouldQuit = true;
                    break;

                default:
                    break;
                }
            }

            if (event.type == SDL_QUIT) {
                m_ShouldQuit = true;
            }
        }

        m_Graphics->ClearScreen();

        update();

        render();

        SDL_Delay(17);
    }
}

void MyGame::cleanup() {

    // Cleanup the ECS Scene
    delete m_Scene;

    m_Assets->Cleanup();

    m_Graphics->Cleanup();

    IMG_Quit();
    TTF_Quit();

    // Close connection to the server
    SDLNet_TCP_Close(m_Socket);

    // Shutdown SDL_net
    SDLNet_Quit();

    // Shutdown SDL
    SDL_Quit();
}

void MyGame::callback_game_send() {

}

void MyGame::callback_game_recv(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    m_Messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
    }
}

void MyGame::update() {
    // Set local player location based on received data
    player1.y = game_data.player1Y;
    m_Scene->m_Transforms[player].m_Rect.y = game_data.player1Y;

    update_kinematics(m_Scene, deltaTime);
}

void MyGame::render() {
    //SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    //SDL_RenderDrawRect(m_Renderer, &player1);


    m_Graphics->DrawScene(m_Scene);
    m_Graphics->Present();
}
