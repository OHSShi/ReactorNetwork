#include	"UDPConnector.hpp"
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>

using namespace OSS::NETWORK;

UDPConnector::UDPConnector()
{
}

UDPConnector::~UDPConnector()
{
}

void	UDPConnector::SetAddress( string rAddress )
{
	mAddress	= rAddress;
}

void	UDPConnector::SetAddress( string rIp , int rPort )
{
	char	rTemp[ 64 ];
	memset( rTemp , 0x00 , sizeof( rTemp ) );

	snprintf( rTemp , sizeof( rTemp ) , "%d" , rPort );

	mAddress	= rIp + ":" + rTemp;
}

string	UDPConnector::GetAddress()
{
	return	mAddress;
}

int		UDPConnector::Open()
{
	int		ret;
	string	rIp;
	string	rPort;
	int		rFound;
	struct sockaddr_in	rAddr;

	rFound	= mAddress.find( ":" );
	if( rFound == string::npos )
	{
		Socket::SetErrorString( "Wrong address format(IP:PORT)" );
		return	-1;
	}

	mSd	= socket( AF_INET , SOCK_DGRAM , 0 );
	if( mSd < 0 )
	{
		Socket::SetErrorString( "socket() fail" );
		return	-1;
	}

	rIp.append( mAddress , 0 , rFound );
	rPort.append( mAddress.begin() + rFound + 1 , mAddress.end() );

	mAddr.sin_family		= AF_INET;
	mAddr.sin_addr.s_addr	= inet_addr( rIp.c_str() );
	mAddr.sin_port			= htons( atoi( rPort.c_str() ) );

#if 0
	int	rOption	= 1;
	setsockopt( mSd , SOL_SOCKET , SO_REUSEADDR , &rOption , sizeof( rOption ) );
#endif

	return	0;
}
