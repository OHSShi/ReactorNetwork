#ifndef		__SOCKET_EVENT_HPP__
#define		__SOCKET_EVENT_HPP__

#include	<iostream>
#include	<string>

using namespace std;

namespace OSS { namespace NETWORK {

class	Socket;

typedef enum
{
	SOCKET_EVENT_ACCEPT		,
	SOCKET_EVENT_RECEIVE	,
	SOCKET_EVENT_DISCONNECT	,
	SOCKET_EVENT_LOST		,
	SOCKET_EVENT_ERROR		
} EVENT_TYPE;

class	SocketEvent
{
	private:
		Socket*		mpSocket;

		EVENT_TYPE	mType;

		int			mNewSd;
	
		char*		mpRecvData;
		int			mRecvBytes;

		int			mErrNo;

	public:
		SocketEvent();
		virtual	~SocketEvent();
	
		void		SetType( EVENT_TYPE	rType );
		EVENT_TYPE	GetType();

		void		SetNewSd( int rSd );
		int			GetNewSd();

		void		SetSocket( Socket* rpSocket );
		Socket*		GetSocket();

		void		SetRecvData( char* rpRecvData , int rRecvBytes );
		char*		GetRecvData();

		int			GetRecvBytes();


		void		SetErrNo( int rErrNo );
		int			GetErrNo();
};

}}

#endif
