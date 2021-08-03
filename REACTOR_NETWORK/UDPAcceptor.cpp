#include	"UDPAcceptor.hpp"
#include	<string.h>

using namespace OSS::NETWORK;

UDPAcceptor::UDPAcceptor()
{

}

UDPAcceptor::~UDPAcceptor()
{

}

int		UDPAcceptor::Open()
{
	int	ret;
	
	mSd	= socket( AF_INET , SOCK_DGRAM , 0 );
	if( mSd < 0 )
	{
		return -1;
	}

	memset( &mAddr , 0x00 , sizeof( mAddr ) );
	mAddr.sin_family    	= AF_INET;
	mAddr.sin_addr.s_addr   = htonl( INADDR_ANY );
	mAddr.sin_port      	= htons( mPortNum );

	ret = bind( mSd , ( struct sockaddr* )&mAddr , sizeof( mAddr ) );
	if( ret < 0 )
	{
		return -1;
	}
	return	0;
}

int		UDPAcceptor::Open( unsigned int rPortNum )
{
	mPortNum	= rPortNum;
	return	Open();
}

void	UDPAcceptor::SetPortNum( unsigned int rPortNum )
{
	mPortNum	= rPortNum;
}

int		UDPAcceptor::GetPortNum()
{
	return	mPortNum;
}
