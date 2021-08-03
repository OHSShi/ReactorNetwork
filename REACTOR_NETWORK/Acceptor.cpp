#include	"Acceptor.hpp"

using namespace OSS::NETWORK;

Acceptor::Acceptor()
{

}

Acceptor::~Acceptor()
{

}

int		Acceptor::Open()
{
	int	ret;

	mSd	= socket( AF_INET , SOCK_STREAM , 0 );
	if( mSd < 0 )
	{
		return -1;
	}

	memset( &mAddr , 0x00 , sizeof( mAddr ) );
	mAddr.sin_family 		= AF_INET;
	mAddr.sin_addr.s_addr 	= htonl( INADDR_ANY );
	mAddr.sin_port 			= htons( mPortNum );

	int	option	= 1;
	setsockopt( mSd , SOL_SOCKET , SO_REUSEADDR , &option , sizeof( option ) );

	ret	= bind( mSd , ( struct sockaddr* )&mAddr , sizeof( mAddr ) );
	if( ret < 0 )
	{
		SetErrNo( errno );
		SetErrorString( "bind error" );
		return -1;
	}
	ret	= listen( mSd , 5 );
	if( ret < 0 )
	{
		SetErrNo( errno );
		SetErrorString( "listen error" );
		return -1;
	}

	return	0;
}

int		Acceptor::Open( unsigned int rPortNum )
{
	mPortNum	= rPortNum;
	return	Open();
}

int		Acceptor::DefineEvent()
{
	int	rSd;
	memset( &mAddr , 0x00 , sizeof( mAddr ) );
	mAddrLen	= sizeof( mAddr );
	rSd	= accept( mSd , ( struct sockaddr* )&mAddr , ( socklen_t* )&mAddrLen );
	if( rSd < 0 )
	{
		mpEvent->SetErrNo( rSd );
		mpEvent->SetType( SOCKET_EVENT_ERROR );
		return -1;
	}

	mpEvent->SetType( SOCKET_EVENT_ACCEPT );
	SetPeerName( inet_ntoa( mAddr.sin_addr ) );
	mpEvent->SetNewSd( rSd );
	
	return	rSd;
}

void	Acceptor::SetPortNum( unsigned int rPortNum )
{
	mPortNum	= rPortNum;
}

int		Acceptor::GetPortNum()
{
	return	mPortNum;
}
