#ifndef		__SOCKET_EVENT_HANDLER_HPP__
#define		__SOCKET_EVENT_HANDLER_HPP__

#include	<iostream>
#include	<string>
#include	"SocketEvent.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class SocketEventHandler
{
	private:

	public:
		virtual	int		ClientAccepted( SocketEvent* rpEvent )	= 0;
		virtual int		DataReceived( SocketEvent* rpEvent )	= 0;
		virtual int		Disconnected( SocketEvent* rpEvent )	= 0;
		virtual int		ConnectionLost( SocketEvent* rpEvent )	= 0;
		virtual int		ErrorHandling( SocketEvent* rpEvent )	= 0;
};

}}

#endif
