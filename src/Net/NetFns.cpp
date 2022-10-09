#include "NetFns.h"
#include "SDL_net.h"
#include "../MyGame.h"
#include <string>
#include <vector>

namespace Net {
	int on_receive(void* engine) {
        MyGame* engineptr = static_cast<MyGame*>(engine);
        TCPsocket socket = engineptr->GetSocket();

        const int message_length = 1024;

        char message[message_length];
        int received;

        // TODO: while(), rather than do
        do {
            received = SDLNet_TCP_Recv(socket, message, message_length);
            message[received] = '\0';

            char* pch = strtok(message, ",");

            // get the command, which is the first string in the message
            std::string cmd(pch);

            // then get the arguments to the command
            std::vector<std::string> args;

            while (pch != NULL) {
                pch = strtok(NULL, ",");

                if (pch != NULL) {
                    args.push_back(std::string(pch));
                }
            }

            engineptr->callback_game_recv(cmd, args);

            if (cmd == "exit") {
                break;
            }

        } while (received > 0 && !engineptr->ShouldQuit());
	}

	int on_send(void* engine) {
        MyGame* engineptr = static_cast<MyGame*>(engine);
        TCPsocket socket = engineptr->GetSocket();

        while (!engineptr->ShouldQuit()) {
            if (engineptr->m_Messages.size() > 0) {

                // TODO: CREATE EVENT CLASS TO SEND EVENT DATA IN A STRUCT?

                std::string message = "CLIENT_DATA";

                for (auto m : engineptr->m_Messages) {
                    message += "," + m;
                }

                engineptr->m_Messages.clear();

                std::cout << "Sending_TCP: " << message << std::endl;

                SDLNet_TCP_Send(socket, message.c_str(), message.length());
            }

            SDL_Delay(1);
        }

        return 0;
	}
}