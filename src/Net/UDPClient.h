#ifndef __UDP_CLIENT_H__
#define __UDP_CLIENT_H__
#include <SDL_net.h>
#include <vector>
#include <string>

namespace Net {
	class UDPClient {
	public:
		UDPClient(const std::string& ip, int32_t remotePort, int32_t localPort);
		~UDPClient();
		UDPpacket* m_Packet;
		UDPpacket* m_RecvPacket;

		UDPsocket& Socket() { return m_Socket; }
		UDPpacket* SendPacket() { return m_Packet; }
		UDPpacket* RecvPacket() { return m_RecvPacket; }
		std::vector<std::string> Messages() { return m_Messages; }
		void AddMessage(std::string message) { m_Messages.push_back(message); }
		std::vector<std::string> m_Messages;
	private:
		UDPsocket m_Socket;
		IPaddress m_ServerIP;
		IPaddress* m_pServerIP;
		IPaddress* m_pLocalIP;
	};
}
#endif