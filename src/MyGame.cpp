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

    m_UDPClient.Connect("localhost", 8888, 0);

    SDL_CreateThread(Net::on_receive_tcp, "ConnectionReceiveThread", (void*)this);
    SDL_CreateThread(Net::on_send_tcp, "ConnectionSendThread", (void*)this);

    SDL_CreateThread(Net::send_udp_packets, "UDPSendThread", (void*)this);

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("SDL Image failed to load!");
        exit(4);
    }

    if (TTF_Init() < 0) {
        printf("SDL TTF Failed to load!");
        exit(4);
    }



    // Engine Init

    m_Graphics = std::make_shared<Graphics>();

    m_Graphics->Init();

    m_Scene = new ECS::Scene();

}

int MyGame::run() {
  
    preload_assets();

    init_entities();

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
    m_Scene->Cleanup();

    AssetManager::Cleanup();

    m_Graphics->Cleanup();

    delete m_Scene;

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

void MyGame::callback_on_connect(std::vector<std::string>& args) {
    if (args.size() == 1) {
        m_NetId = stoi(args.at(0));
        std::cout << "Player ID: " << m_NetId << std::endl;
    }
    else {
        std::cerr << "Invalid On Connect Data Received!" << std::endl;
        for (auto& arg : args) {
            std::cerr << arg << std::endl;
        }
    }
}

void MyGame::callback_game_recv(std::vector<std::string>& args) {
        // we should have exactly 6 arguments
        if (args.size() == 4) {
            game_data.player1X = stoi(args.at(0));
            game_data.player1Y = stoi(args.at(1));
            game_data.player2X = stoi(args.at(2));
            game_data.player2Y = stoi(args.at(3));
        }
        else {
            std::cerr << "Invalid Game State data received!" << std::endl;
            for (auto& arg : args) {
                std::cerr << arg << std::endl;
            }
        }
}

void MyGame::callback_update_scores(std::vector<std::string>& args) {
    if (args.size() == 2) {
        game_data.score1 = stoi(args.at(0));
        game_data.score2 = stoi(args.at(1));
    }
}

void MyGame::send(std::string message) {
    m_Messages.push_back(message);
}

void MyGame::send_udp(std::string message) {
    m_UDPClient.m_Messages.push_back(message);
}

void MyGame::preload_assets() {
    AssetManager::LoadFont("Score", "res/fonts/lato.ttf", 32);
    AssetManager::loadTexture("Paddle", "res/sprites/paddle.png", m_Graphics.get());
    AssetManager::loadTexture("Ball", "res/sprites/ball.png", m_Graphics.get());
}

void MyGame::init_entities() {
    player1 = m_Scene->NewEntity();
    m_Scene->AddComponents(player1, CompTags::Transform | CompTags::Sprite);
    m_Scene->m_Transforms[player1].m_Position = { 200, 0 };
    m_Scene->m_Transforms[player1].m_Scale = { 20, 60 };
    m_Scene->m_SpriteRenders[player1].m_Sprite = AssetManager::Textures.at("Paddle");

    player2 = m_Scene->NewEntity();
    m_Scene->AddComponents(player2, CompTags::Transform | CompTags::Sprite);
    m_Scene->m_Transforms[player2].m_Position = { 600, 0 };
    m_Scene->m_Transforms[player2].m_Scale = { 20, 60 };
    m_Scene->m_SpriteRenders[player2].m_Sprite = AssetManager::Textures.at("Paddle");

    scoreText1 = m_Scene->NewEntity();
    m_Scene->AddComponents(scoreText1, CompTags::Text | CompTags::Transform);
    m_Scene->m_Transforms[scoreText1].m_Position = { 100, 100 };
    m_Scene->m_Texts[scoreText1].m_Text = "Player 1: 0";
    m_Scene->m_Texts[scoreText1].m_Font = AssetManager::Fonts.at("Score");
    m_Scene->m_Texts[scoreText1].m_Color = { 0xFF, 0xFF, 0xFF, 0xFF };


    scoreText2 = m_Scene->NewEntity();
    m_Scene->AddComponents(scoreText2, CompTags::Text | CompTags::Transform);
    m_Scene->m_Transforms[scoreText2].m_Position = { 500, 100 };
    m_Scene->m_Texts[scoreText2].m_Text = "Player 2: 0";
    m_Scene->m_Texts[scoreText2].m_Font = AssetManager::Fonts.at("Score");
    m_Scene->m_Texts[scoreText2].m_Color = { 0xFF, 0xFF, 0xFF, 0xFF };
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "W_DOWN") : (std::to_string(m_NetId)+"W_UP"));
            break;
        /*case SDLK_s:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "S_DOWN") : (std::to_string(m_NetId) + "S_UP"));
            break;*/
        case SDLK_a:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "A_DOWN") : (std::to_string(m_NetId) + "A_UP"));
            break;
        case SDLK_d:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "D_DOWN") : (std::to_string(m_NetId) + "D_UP"));
            break;
        case SDLK_j:
          send_udp(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "J_DOWN") : (std::to_string(m_NetId) + "J_UP"));
    }
}

void MyGame::update() {
    // Set local player location based on received data
    //player1.y = game_data.player1Y;
    m_Scene->m_Transforms[player1].m_Position.x = game_data.player1X;
    m_Scene->m_Transforms[player1].m_Position.y = game_data.player1Y;
    m_Scene->m_Transforms[player2].m_Position.x = game_data.player2X;
    m_Scene->m_Transforms[player2].m_Position.y = game_data.player2Y;

    m_Scene->m_Texts[scoreText1].m_Text = "Player 1: " + std::to_string(game_data.score1);
    m_Scene->m_Texts[scoreText2].m_Text = "Player 2: " + std::to_string(game_data.score2);

    update_kinematics(m_Scene, deltaTime);
}

void MyGame::render() {
    //SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    //SDL_RenderDrawRect(m_Renderer, &player1);

    m_Graphics->DrawScene(m_Scene);
    m_Graphics->Present();
}
