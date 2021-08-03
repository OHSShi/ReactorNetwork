#ifndef		__SOCKET_HPP__
#define		__SOCKET_HPP__

#include	<iostream>
#include	<string>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<time.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	"SocketEventHandler.hpp"
#include	"SocketEvent.hpp"

using namespace std;

namespace OSS { namespace NETWORK {


class	Socket
{
	protected:
		unsigned int	mSd;
		string			mPeerName;
		
		SocketEventHandler*	mpEventHandler;
		SocketEvent*		mpEvent;

		string			mErrorString;	
		int				mErrNo;

	private:
	public:
		Socket();
		virtual ~Socket();

		void			SetSd( unsigned int rSd );
		unsigned int	GetSd();

		void			SetPeerName( string rPeerName );
		string			GetPeerName();

		void				SetEventHandler( SocketEventHandler*	rpEventHandler );
		SocketEventHandler*	GetEventHandler();

		void			SetEvent( SocketEvent* rpEvent );
		SocketEvent*	GetEvent();

		virtual int		Open()			= 0;
		virtual int		DefineEvent()	= 0;

		int				CallEventHandler();
		int				Close();

		int				TransNonBlock();
		int				TransBlock();

		int				SetFdWriteTimeout( struct timeval rTimeVal );

		void			SetErrorString( string rErrorString );
		string			GetErrorString();

		void			SetErrNo( int rErrNo );
		int				GetErrNo();

};

}}

#endif
