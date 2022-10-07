#include "SDL_net.h"
#include "MyGame.h"

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


    // Engine Init

    // Window
    m_Window = SDL_CreateWindow(
        "Multiplayer Pong Client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!m_Window) {
        std::cerr << "Failed to create window" << SDL_GetError() << std::endl;
    }

    // Renderer
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_Renderer) {
        std::cerr << "Failed to create renderer" << SDL_GetError() << std::endl;
    }
}

int MyGame::run() {
  
    main_loop();

    return 0;
}

void MyGame::main_loop() {
    SDL_Event event;

    while (!m_ShouldQuit) {
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

        SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_Renderer);

        update();

        render();

        SDL_RenderPresent(m_Renderer);

        SDL_Delay(17);
    }
}

void MyGame::cleanup() {

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
    player1.y = game_data.player1Y;
}

void MyGame::render() {
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(m_Renderer, &player1);
}
