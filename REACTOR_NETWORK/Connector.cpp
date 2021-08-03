#include	"Connector.hpp"
using namespace OSS::NETWORK;

Connector::Connector()
{

}

Connector::~Connector()
{

}

int		Connector::Open()
{
	int	ret;
	string	rIp;
	string	rPort;
	int		rFound;
	struct sockaddr_in	rAddr;

	rFound	= mAddress.find( ":" );
	if( rFound == string::npos )
	{
		Socket::SetErrorString( "Wrong address format(IP:PORT)" ); 
		return -1;
	}

	mSd	= socket( AF_INET , SOCK_STREAM , 0 );
	if( mSd < 0 )
	{
		Socket::SetErrorString( "socket() fail" );
		return -1;
	}
	
	rIp.append( mAddress , 0 , rFound );
	rPort.append( mAddress.begin() + rFound + 1 , mAddress.end() );

	rAddr.sin_family		= AF_INET;
	rAddr.sin_addr.s_addr	= inet_addr( rIp.c_str() );
	rAddr.sin_port			= htons( atoi( rPort.c_str() ) );

	int option  = 1;
	setsockopt( mSd , SOL_SOCKET , SO_REUSEADDR , &option , sizeof( option ) );

	int	rCliLen	= sizeof( rAddr );

	ret	= connect( mSd , ( struct sockaddr* )&rAddr , rCliLen );
	if( ret < 0 )
	{
		Socket::SetErrorString( "connect() fail" );
		return -1;
	}
	
	return	0;
	
}

int		Connector::SetAddress( string rAddress )
{
	mAddress	= rAddress;

}

string	Connector::GetAddress()
{
	return	mAddress;	
}
