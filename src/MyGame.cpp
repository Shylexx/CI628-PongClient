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


    // Load any assets before doing networking
    preload_assets();

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

    SDL_CreateThread(Net::on_receive_tcp, "ConnectionReceiveThread", (void*)this);
    SDL_CreateThread(Net::on_send_tcp, "ConnectionSendThread", (void*)this);

}

int MyGame::run() {
  

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
deltaTime = ((m_Now - m_Last) * 1000 / (double)SDL_GetPerformanceFrequency());

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

void MyGame::callback_load_level(std::vector<std::string>& args) {
  std::cout << "Loading level!" << std::endl;
  int height = std::stoi(args.at(0));
  int width = std::stoi(args.at(1));
  int arg = 2;
  std::vector<std::vector<ECS::TileType>> tiles;
  for (int row = 0; row < height; row++) {
    std::cout << "loading row: " << row << std::endl;
    tiles.push_back(std::vector<ECS::TileType>());
    for (int col = 0; col < width; col++) {
      ECS::TileType tileType = static_cast<ECS::TileType>(std::stoi(args.at(arg)));
      if (tileType == ECS::TileType::WALL) {
        std::cout << "wall added" << std::endl;
      }
      tiles[row].push_back(tileType);
      arg++;
    }
  }
  m_Scene->m_Tilemaps[level].m_Tiles = std::move(tiles);
  std::cout << "Level Loaded!" << std::endl;
  std::cout << m_Scene->m_Tilemaps[level].m_Tiles[30][10] << std::endl;
}

void MyGame::callback_update_level(std::vector<std::string>& args) {
  if (args.size() % 3 == 0) {
  std::cout << "Updating level!" << std::endl;
    for (int i = 0; i < args.size() / 3; i += 3) {
      m_Scene->m_Tilemaps[level].setTile(static_cast<ECS::TileType>(std::stoi(args[i])), std::stoi(args[i+1]), std::stoi(args[i + 2]));
     //std::cout << "Update info - New tile type: " << std::stoi(args[i]) << " at coords: X - " << args[i + 1] << " Y - " << args[i + 2] << std::endl;
    }
  }
}

void MyGame::callback_spawn_entity(std::vector<std::string>& args) {

}

void MyGame::callback_kill_entity(std::vector<std::string>& args) {

}

void MyGame::send(std::string message) {
    m_Messages.push_back(message);
}

void MyGame::preload_assets() {
    AssetManager::LoadFont("Score", "res/fonts/lato.ttf", 32);
    AssetManager::loadTexture("Paddle", "res/sprites/paddle.png", m_Graphics.get());
    AssetManager::loadTexture("Ball", "res/sprites/ball.png", m_Graphics.get());
    AssetManager::loadTexture("Wall", "res/sprites/walltile.png", m_Graphics.get());
    AssetManager::loadTexture("Empty", "res/sprites/floortile.png", m_Graphics.get());
    AssetManager::loadTexture("Player", "res/sprites/playersprite.png", m_Graphics.get());
    AssetManager::loadTexture("Bullet", "res/sprites/bulletsprite.png", m_Graphics.get());
}

void MyGame::init_entities() {
    player1 = m_Scene->NewEntity();
    m_Scene->AddComponents(player1, CompTags::Transform | CompTags::Sprite);
    m_Scene->m_Transforms[player1].m_Position = { 200, 0 };
    m_Scene->m_Transforms[player1].m_Scale = { 16, 16 };
    m_Scene->m_SpriteRenders[player1].m_Sprite = AssetManager::Textures.at("Player");

    player2 = m_Scene->NewEntity();
    m_Scene->AddComponents(player2, CompTags::Transform | CompTags::Sprite);
    m_Scene->m_Transforms[player2].m_Position = { 600, 0 };
    m_Scene->m_Transforms[player2].m_Scale = { 16, 16 };
    m_Scene->m_SpriteRenders[player2].m_Sprite = AssetManager::Textures.at("Player");

    scoreText1 = m_Scene->NewEntity();
    m_Scene->AddComponents(scoreText1, CompTags::Text | CompTags::Transform);
    m_Scene->m_Transforms[scoreText1].m_Position = { 500, 100 };
    m_Scene->m_Texts[scoreText1].m_Text = "Player 2: 0";
    m_Scene->m_Texts[scoreText1].m_Font = AssetManager::Fonts.at("Score");
    m_Scene->m_Texts[scoreText1].m_Color = { 0xFF, 0xFF, 0xFF, 0xFF };


    scoreText2 = m_Scene->NewEntity();
    m_Scene->AddComponents(scoreText2, CompTags::Text | CompTags::Transform);
    m_Scene->m_Transforms[scoreText2].m_Position = { 100, 100 };
    m_Scene->m_Texts[scoreText2].m_Text = "Player 1: 0";
    m_Scene->m_Texts[scoreText2].m_Font = AssetManager::Fonts.at("Score");
    m_Scene->m_Texts[scoreText2].m_Color = { 0xFF, 0xFF, 0xFF, 0xFF };

    level = m_Scene->NewEntity();
    m_Scene->AddComponents(level, CompTags::Tilemap | CompTags::Transform);
    m_Scene->m_Tilemaps[level].setEmpty();
    m_Scene->m_Tilemaps[level].m_WallTexture = AssetManager::Textures.at("Wall");
    m_Scene->m_Tilemaps[level].m_EmptyTexture = AssetManager::Textures.at("Empty");
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "W_DOWN") : (std::to_string(m_NetId)+"W_UP"));
            if (m_NetId == 1) {
              m_Scene->m_Transforms[player1].m_Rotation = 270;
            }
            else if (m_NetId == 2) {
              m_Scene->m_Transforms[player2].m_Rotation = 270;
            }
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "S_DOWN") : (std::to_string(m_NetId) + "S_UP"));
            if (m_NetId == 1) {
              m_Scene->m_Transforms[player1].m_Rotation = 90;
            }
            else if (m_NetId == 2) {
              m_Scene->m_Transforms[player2].m_Rotation = 90;
            }
            break;
        case SDLK_a:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "A_DOWN") : (std::to_string(m_NetId) + "A_UP"));
            if (m_NetId == 1) {
              m_Scene->m_Transforms[player1].m_Rotation = 180;
            }
            else if (m_NetId == 2) {
              m_Scene->m_Transforms[player2].m_Rotation = 180;
            }
            break;
        case SDLK_d:
            send(event.type == SDL_KEYDOWN ? (std::to_string(m_NetId) + "D_DOWN") : (std::to_string(m_NetId) + "D_UP"));
            if (m_NetId == 1) {
              m_Scene->m_Transforms[player1].m_Rotation = 0;
            }
            else if (m_NetId == 2) {
              m_Scene->m_Transforms[player2].m_Rotation = 0;
            }
            break;
        case SDLK_SPACE:
						if (event.type == SDL_KEYDOWN) {
							send(std::to_string(m_NetId) + "F_DOWN");
						}
						break;
    }
}

void MyGame::update() {
    // Set local player location based on received data
    //player1.y = game_data.player1Y;
    m_Scene->m_Transforms[player1].m_Position.x = game_data.player1X;
    m_Scene->m_Transforms[player1].m_Position.y = game_data.player1Y;
    m_Scene->m_Transforms[player2].m_Position.x = game_data.player2X;
    m_Scene->m_Transforms[player2].m_Position.y = game_data.player2Y;

    m_Scene->m_Texts[scoreText2].m_Text = "Player 1: " + std::to_string(game_data.score2);
    m_Scene->m_Texts[scoreText1].m_Text = "Player 2: " + std::to_string(game_data.score1);

}

void MyGame::render() {
    //SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    //SDL_RenderDrawRect(m_Renderer, &player1);

    m_Graphics->DrawScene(m_Scene);
    m_Graphics->Present();
}
