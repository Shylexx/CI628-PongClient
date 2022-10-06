#include "NetFns.h"
#include "SDL_net.h"

namespace Net {
	int on_receive(void* socket_ptr) {

	}

	int on_send(void* socket_ptr) {
        TCPsocket socket = (TCPsocket)socket_ptr;

        while (is_running) {
            if (game->messages.size() > 0) {
                string message = "CLIENT_DATA";

                for (auto m : game->messages) {
                    message += "," + m;
                }

                game->messages.clear();

                cout << "Sending_TCP: " << message << endl;

                SDLNet_TCP_Send(socket, message.c_str(), message.length());
            }

            SDL_Delay(1);
        }

        return 0;
	}
}