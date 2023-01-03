#include "UDPClient.h"
#include <iostream>

namespace Net {
	UDPClient::UDPClient(const std::string& ip, int32_t remotePort, int32_t localPort) {
		std::cout << "Connecting to IP: " << ip << " Port: " << remotePort << std::endl;
		if (!(m_Socket = SDLNet_UDP_Open(0))) {
			std::cerr << "Could not create UDP Socket!" << std::endl;
			return;
		}

		m_pLocalIP = SDLNet_UDP_GetPeerAddress(m_Socket, -1);
		if (!m_pLocalIP) {
			std::cerr << "Could not get our port!" << std::endl;
			return;
		}
		else {
			std::cout << "Local Address: " << m_pLocalIP->host << std::endl;
			std::cout << "Local Port: " << m_pLocalIP->port << std::endl;
		}

		// Set up recv packet
		m_RecvPacket = SDLNet_AllocPacket(512);
		if (!m_RecvPacket) {
			std::cerr << "Could not allocate Receive UDP Packet" << std::endl;
			return;
		}

		// Resolve server address
		m_pServerIP = &m_ServerIP;
		if(SDLNet_ResolveHost(m_pServerIP, "localhost", 8888)) {
			std::cerr << "Could not resolve host!" << std::endl;
		}

		// Set up our outgoing packet
		m_Packet = SDLNet_AllocPacket(512);
		if (!m_Packet) {
			std::cerr << "Could not allocate send UDP packet" << std::endl;
			return;
		}

		// Link the server address to our sending packet
		m_Packet->address.host = m_pServerIP->host;
		m_Packet->address.port = m_pServerIP->port;

	}

	UDPClient::~UDPClient() {
		SDLNet_FreePacket(m_Packet);
		SDLNet_FreePacket(m_RecvPacket);
		SDLNet_UDP_Close(m_Socket);
	}
}