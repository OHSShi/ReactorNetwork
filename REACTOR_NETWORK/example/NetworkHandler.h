#ifndef		__NETWORK_HANDLER_HPP__
#define		__NETWORK_HANDLER_HPP__

#include	<iostream>
#include	<string>
#include	"../Multiplexer.hpp"
#include	"../SocketEventHandler.hpp"
#include	"../Client.hpp"
#include	"../Acceptor.hpp"

using namespace std;
using namespace OSS::NETWORK;

class NetworkHandler : public SocketEventHandler , public TimeoutEventHandler
{
	private:
		Multiplexer*	mpMultiplexer;

		SocketEvent*	mpSharedEvent;
		char*			mpSharedBuffer;
		int				mSharedBufferSize;

		int				mPortNum;
		Acceptor		mAcceptor;

		pthread_t		mThreadId;

		int				mTimeOutSec;

	public:
		NetworkHandler();
		virtual ~NetworkHandler();

		int     ClientAccepted( SocketEvent* rpEvent );
		int     DataReceived( SocketEvent* rpEvent );
		int     Disconnected( SocketEvent* rpEvent );
		int     ConnectionLost( SocketEvent* rpEvent );
		int     TimedOut();
		int     ErrorHandling( SocketEvent* rpEvent );

		void	SetPort( int rPort );
		int		Run();
		int		Init();

		void	WorkingThread();

};

#endif
