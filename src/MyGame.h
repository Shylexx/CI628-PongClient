#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_net.h"

#include "Engine/Graphics.h"
#include "ECS/ECS.h"
#include "Engine/AssetManager.h"
#include "Net/UDPClient.h"

// Forward Declare networking functions
namespace Net {
    int on_send_tcp(void* engine);
    int on_receive_tcp(void* engine);
}

static struct GameData {
    int player1X = 0;
    int player1Y = 0;
    int player2X = 0;
    int player2Y = 0;
    int score1 = 0;
    int score2 = 0;
} game_data;

class MyGame {

    private:
        //SDL_Rect player1 = { 0, 0, 20, 60 };
        Entity player1;
        Entity player2;
        Entity scoreText1;
        Entity scoreText2;
        Entity level;
        
        const char* IP_NAME = "localhost";
        const Uint16 PORT = 55555;

        bool m_ShouldQuit = false;
        TCPsocket m_Socket;
        
        void preload_assets();
        void init_entities();
        void main_loop();
        void input(SDL_Event& event);
        void update();
        void render();

        // Timekeeping
        uint64_t m_Now = SDL_GetPerformanceCounter();
        uint64_t m_Last = 0;
        float deltaTime = 0;

        // ECS Scene holds data of all objects in the scene
        ECS::Scene* m_Scene;

        std::shared_ptr<Graphics> m_Graphics;

        int m_NetId = 0;

    public:
        MyGame();
        std::vector<std::string> m_Messages;
        int run();
        
        void cleanup();

        // Networking functions
        // Runs when we send a message to the server from the client
        void callback_game_send();
        // Runs when the client receives Game transform data from the server
        void callback_game_recv(std::vector<std::string>& args);
        // Runs when the client receives initial game data for setup
        void callback_on_connect(std::vector<std::string>& args);
        // updates the scores when a player scores
        void callback_update_scores(std::vector<std::string>& args);
        // loads a level when new level data is received
        void callback_load_level(std::vector<std::string>& args);
        // updates tiles in an already loaded level
        void callback_update_level(std::vector<std::string>& args);
        // Queues a message to be sent in the next frame
        void send(std::string message);

        // Accessors for private members
        TCPsocket GetSocket() { return m_Socket; }
        bool ShouldQuit() { return m_ShouldQuit; }
        int NetID() { return m_NetId; }
};

#endif