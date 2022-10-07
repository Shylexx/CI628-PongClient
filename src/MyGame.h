#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_net.h"

// Forward Declare networking functions
namespace Net {
    int on_send(void* engine);
    int on_receive(void* engine);
}

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

class MyGame {

    private:
        SDL_Rect player1 = { 0, 0, 20, 60 };
        
        SDL_Renderer* m_Renderer;
        const char* IP_NAME = "localhost";
        const Uint16 PORT = 55555;

        bool m_ShouldQuit = false;
        TCPsocket m_Socket;
        SDL_Window* m_Window;
        
        void main_loop();
        void input(SDL_Event& event);
        void update();
        void render();

    public:
        MyGame();
        std::vector<std::string> m_Messages;
        int run();
        
        void cleanup();

        // Networking functions
        // Runs when we send a message to the server from the client
        void callback_game_send();
        // Runs when the client receives a message from the server
        void callback_game_recv(std::string message, std::vector<std::string>& args);
        // Queues a message to be sent in the next frame
        void send(std::string message);

        // Accessors for private members
        TCPsocket GetSocket() { return m_Socket; }
        SDL_Window* GetWindow() { return m_Window; }
        bool ShouldQuit() { return m_ShouldQuit; }
};

#endif