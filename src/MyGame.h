#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_net.h"
#include "NetFns.h"

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

class MyGame {

    private:
        SDL_Rect player1 = { 0, 0, 20, 60 };
        bool m_ShouldQuit = false;
        SDL_Renderer* m_Renderer;
        const char* IP_NAME = "localhost";
        const Uint16 PORT = 55555;
        TCPsocket m_Socket;

    public:
        MyGame();
        std::vector<std::string> messages;

        
        void input(SDL_Event& event);
        void update();
        void render();

        int run();
        void main_loop();
        void cleanup();

        void callback_game_send();
        void callback_game_recv(std::string message, std::vector<std::string>& args);

        // Static SDL functions
        
        void send(std::string message);
};

#endif