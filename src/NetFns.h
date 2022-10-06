#ifndef __NET_FNS__
#define __NET_FNS__

namespace Net {
	int on_send(void* socket_ptr);
	int on_receive(void* socket_ptr);
}
#endif
