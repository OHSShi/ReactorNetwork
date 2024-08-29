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
		Socket::SetErrNo( errno );
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
		Socket::SetErrNo( errno );
		Socket::SetErrorString( "connect() fail" );
		return -1;
	}
	
	return	0;
	
}

int     Connector::Open( int rTimeOutSec )
{
    int ret;
    string  rIp;
    string  rPort;
    int     rFound;
    struct sockaddr_in  rAddr;

    rFound  = mAddress.find( ":" );
    if( ( size_t )rFound == string::npos )
    {
        Socket::SetErrorString( "Invliad Address" );
        return -1;
    }

    mSd = socket( AF_INET , SOCK_STREAM , 0 );
    if( mSd < 0 )
    {
        Socket::SetErrorString( "socket() Fail" );
        return -1;
    }

    Socket::TransNonBlock();

    rIp.append( mAddress , 0 , rFound );
    rPort.append( mAddress.begin() + rFound + 1 , mAddress.end() );

    rAddr.sin_family        = AF_INET;
    rAddr.sin_addr.s_addr   = inet_addr( rIp.c_str() );
    rAddr.sin_port          = htons( atoi( rPort.c_str() ) );

    int option  = 1;
    setsockopt( mSd , SOL_SOCKET , SO_REUSEADDR , &option , sizeof( option ) );

    int rCliLen = sizeof( rAddr );

    char    rTemp[ 1024 ];
    memset( rTemp , 0x00 , sizeof( rTemp ) );

    ret = connect( mSd , ( struct sockaddr* )&rAddr , rCliLen );
    if( ret < 0 )
    {
        if( errno == EINPROGRESS )
        {
            struct timeval tv;
            tv.tv_sec   = rTimeOutSec;
            tv.tv_usec  = 0;

            fd_set  rWriteSet;

            FD_ZERO( &rWriteSet );
            FD_SET( mSd , &rWriteSet );

            while( 1 )
            {
                ret = select( mSd + 1  , NULL , &rWriteSet , NULL , &tv );
                if( ret < 0 && errno != EINTR )
                {
                    snprintf( rTemp , sizeof( rTemp ) , "select() error. errno(%d)" , errno );
                    Socket::SetErrorString( rTemp );
                    return  -1;
                }
                else if( ret == 0 )
                {
                    Socket::SetErrorString( "Timed Out" );
                    return  -1;
                }
		else
                {
                    int error   = 0;
                    socklen_t   len = sizeof( error );
                    if( getsockopt( mSd , SOL_SOCKET , SO_ERROR , &error , &len ) < 0 )
                    {
                        snprintf( rTemp , sizeof( rTemp ) , "getsockopt() error. errno(%d)" , errno );
                        Socket::SetErrorString( rTemp );
                        return  -1;
                    }

                    if( error )
                    {
                        snprintf( rTemp , sizeof( rTemp ) , "Error in delayed connection. errno(%d)" , error );
                        Socket::SetErrorString( rTemp );
                        return  -1;

                    }

                    break;
                }
            }
        }
        else
        {
            Socket::SetErrorString( "Connect Fail" );
            return -1;
        }
    }

    return  0;

}

int		Connector::SetAddress( string rAddress )
{
	mAddress	= rAddress;

}

string	Connector::GetAddress()
{
	return	mAddress;	
}
