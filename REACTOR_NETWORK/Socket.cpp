#include	"Socket.hpp"

using namespace OSS::NETWORK;

Socket::Socket()
{

}

Socket::~Socket()
{

}

void	Socket::SetSd( unsigned int rSd )
{
	mSd	= rSd;
}

unsigned int Socket::GetSd()
{
	return mSd;
}

void	Socket::SetPeerName( string rPeerName )
{
	mPeerName	= rPeerName;
}

string	Socket::GetPeerName()
{
	return mPeerName;
}

void	Socket::SetEventHandler( SocketEventHandler* rpEventHandler )
{
	mpEventHandler	= rpEventHandler;
}

SocketEventHandler*	Socket::GetEventHandler()
{
	return mpEventHandler;
}

void	Socket::SetEvent( SocketEvent* rpEvent )
{
	mpEvent	= rpEvent;
}

SocketEvent*	Socket::GetEvent()
{
	return mpEvent;
}


int		Socket::CallEventHandler()
{
	switch( mpEvent->GetType() )
	{
		case SOCKET_EVENT_ACCEPT :
			return	mpEventHandler->ClientAccepted( mpEvent );
			break;
		case SOCKET_EVENT_RECEIVE :
			return	mpEventHandler->DataReceived( mpEvent );
			break;
		case SOCKET_EVENT_DISCONNECT :
			return	mpEventHandler->Disconnected( mpEvent );
			break;
		case SOCKET_EVENT_LOST :
			return	mpEventHandler->ConnectionLost( mpEvent );
			break;
		case SOCKET_EVENT_ERROR :
			return	mpEventHandler->ErrorHandling( mpEvent );
			break;
		default:
			return -1;
			break;

	}
}

int		Socket::Close()
{
	close( mSd );
}

int		Socket::TransNonBlock()
{
	int		opts;
	opts	= fcntl( mSd , F_GETFL );
	if( opts < 0 )
	{
		return	-1;
	}
	opts	= ( opts | O_NONBLOCK );
	if( fcntl( mSd , F_SETFL , opts ) < 0 )
	{
		return	-1;
	}
	return 0;
}

int		Socket::TransBlock()
{
	int opts;
	opts = fcntl( mSd , F_GETFL );
	if(opts < 0)
	{
		return -1;
	}
	opts &= ~O_NONBLOCK;
	if( fcntl( mSd , F_SETFL, opts ) < 0 )
	{
		return -1;
	}
	return 0;
}

int		Socket::SetFdWriteTimeout( struct timeval rTimeVal )
{
	int	ret;
	ret	= setsockopt( mSd , SOL_SOCKET , SO_SNDTIMEO , &rTimeVal , sizeof( rTimeVal ) );
	
	return	ret;
}

void	Socket::SetErrorString( string rErrorString )
{
	mErrorString	= rErrorString;
}

string	Socket::GetErrorString()
{
	return	mErrorString;
}

void	Socket::SetErrNo( int rErrNo )
{
	mErrNo	= rErrNo;
}

int		Socket::GetErrNo()
{
	return	mErrNo;
}
