#include "NetFns.h"
#include "SDL_net.h"
#include "../MyGame.h"
#include <string>
#include <vector>

namespace Net {
	int on_receive_tcp(void* engine) {
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

   
            if (cmd == "GAME_DATA") {
                engineptr->callback_game_recv(args);
            }
            else if (cmd == "ON_CONNECT") {
                engineptr->callback_on_connect(args);
            }
            else if (cmd == "SCORES") {
                engineptr->callback_update_scores(args);
            }

            if (cmd == "exit") {
                break;
            }

        } while (received > 0 && !engineptr->ShouldQuit());
	}

	int on_send_tcp(void* engine) {
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

  int send_udp_packets(void* engine) {
    MyGame* engineptr = static_cast<MyGame*>(engine);
    UDPsocket& socket = engineptr->m_UDPClient.Socket();

    while (!engineptr->ShouldQuit()) {
      if (engineptr->m_UDPClient.m_Messages.size() > 0) {
         
        std::string message = "TEST PACKET";

        //for (auto m : engineptr->m_UDPClient.m_Messages) {
        //  message += "," + m;
        //}

        engineptr->m_UDPClient.m_Messages.clear();

        std::cout << "Sending UDP Message: " << message << std::endl;

        memcpy(engineptr->m_UDPClient.m_Packet->data, message.c_str(), message.size());
        engineptr->m_UDPClient.m_Packet->len = message.size();
        SDLNet_UDP_Send(socket, -1, engineptr->m_UDPClient.m_Packet);

        std::cout << "with String data: " << engineptr->m_UDPClient.m_Packet->data << " and size: " << message.size() << std::endl;
        std::cout << "Packet size is: " << engineptr->m_UDPClient.m_Packet->len << std::endl;
      }
    }

    return 0;
  }

  int recv_udp_packet(void* engine) {
    MyGame* engineptr = static_cast<MyGame*>(engine);
    UDPsocket& socket = engineptr->m_UDPClient.Socket();

    while (!engineptr->ShouldQuit()) {
      if (SDLNet_UDP_Recv(socket, engineptr->m_UDPClient.m_RecvPacket)) {
        std::cout << "Received UDP Packet from " << engineptr->m_UDPClient.m_RecvPacket->address.host << " port: " << engineptr->m_UDPClient.m_RecvPacket->address.port << std::endl;
        std::string message((char*)engineptr->m_UDPClient.m_RecvPacket->data);
        std::cout << message << std::endl;
        std::cout << "End of UDP Data" << std::endl;
      }
    }

    return 0;
  }
}