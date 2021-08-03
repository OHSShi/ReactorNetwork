#include	"UDPStream.hpp"
#include	<string.h>
#include	<sys/poll.h>
#include	<sys/time.h>

using namespace OSS::NETWORK;

UDPStream::UDPStream()
{

}

UDPStream::~UDPStream()
{

}

void	UDPStream::SetBuffer( char* rpBuffer , int rSize )
{
	mpBuffer	= rpBuffer;
	mBufferSize	= rSize;
}

char*	UDPStream::GetBuffer()
{
	return	mpBuffer;
}

int		UDPStream::GetBufferSize()
{
	return	mBufferSize;
}

int		UDPStream::Send( char* rpData , int rSize )
{
	return	sendto( mSd , rpData , rSize , 0 , ( struct sockaddr* )&mAddr , sizeof( mAddr ) );
}

int		UDPStream::Send( const string& rData )
{
	return	sendto( mSd , ( char* )rData.c_str() , rData.size() , 0 , ( struct sockaddr* )&mAddr , sizeof( mAddr ) );

}

int		UDPStream::Transaction( char* rpData , int rSize , char* rpRecvBuff , int rRecvBuffSize , int rTimeoutMilliSec )
{
	int	ret	= sendto( mSd , rpData , rSize , 0 , ( struct sockaddr* )&mAddr , sizeof( mAddr ) );
	if( ret < 0 )
	{
		return	ret;
	}

	struct pollfd	rPollFd;
	int rTimer   = rTimeoutMilliSec;
	int rTimeDiffSec;
	int rTimeDiffUSec;
	int rTimeDiff;

	rPollFd.fd		= mSd;
	rPollFd.events	= POLLIN | POLLNVAL;

	while( 1 )
	{
		struct timeval rBeforeTime;
		gettimeofday( &rBeforeTime , NULL );

		ret	= poll( &rPollFd , 1 , rTimer );
		if( ret < 0 )
		{
			if( errno == EINTR )
			{
				struct timeval  rAfterTime;
				gettimeofday( &rAfterTime , NULL );

				rTimeDiffSec    = rAfterTime.tv_sec - rBeforeTime.tv_sec;
				rTimeDiffUSec   = rAfterTime.tv_usec - rBeforeTime.tv_usec;
				rTimeDiff       = ( rTimeDiffSec * 1000 ) + ( rTimeDiffUSec / 1000 );

				if( rTimeDiff >= rTimer )
				{
					return	0;	// Timeout.
				}
				else
				{
					rTimer  = rTimer - ( rTimeDiff );
				}
				continue;
			}
			return	-1;
		}
		else if( ret == 0 )	// Timeout.
		{
			return	0;
		}

		if( rPollFd.revents & POLLNVAL )
		{
			return	-1;
		}

		int	rAddrLen	= sizeof( mAddr );
		ret =recvfrom( mSd , rpRecvBuff , rRecvBuffSize , 0 , ( struct sockaddr* )&mAddr , ( socklen_t* )&rAddrLen );
		
		return	ret;
	}
}

int		UDPStream::Transaction( char* rpData , int rSize , char* rpRecvBuff , int rRecvBuffSize , int* rpTimeoutMilliSec )
{
	int	ret	= sendto( mSd , rpData , rSize , 0 , ( struct sockaddr* )&mAddr , sizeof( mAddr ) );
	if( ret < 0 )
	{
		return	ret;
	}

	struct pollfd	rPollFd;
	int rTimer   = *rpTimeoutMilliSec;
	int rTimeDiffSec;
	int rTimeDiffUSec;
	int rTimeDiff;

	rPollFd.fd		= mSd;
	rPollFd.events	= POLLIN | POLLNVAL;

	while( 1 )
	{
		struct timeval rBeforeTime;
		gettimeofday( &rBeforeTime , NULL );

		ret	= poll( &rPollFd , 1 , rTimer );
		if( ret < 0 )
		{
			if( errno == EINTR )
			{
				struct timeval  rAfterTime;
				gettimeofday( &rAfterTime , NULL );

				rTimeDiffSec    = rAfterTime.tv_sec - rBeforeTime.tv_sec;
				rTimeDiffUSec   = rAfterTime.tv_usec - rBeforeTime.tv_usec;
				rTimeDiff       = ( rTimeDiffSec * 1000 ) + ( rTimeDiffUSec / 1000 );

				if( rTimeDiff >= rTimer )
				{
					return	0;	// Timeout.
				}
				else
				{
					rTimer  = rTimer - ( rTimeDiff );
				}
				continue;
			}
			return	-1;
		}
		else if( ret == 0 )	// Timeout.
		{
			return	0;
		}

		if( rPollFd.revents & POLLNVAL )
		{
			return	-1;
		}

		int	rAddrLen	= sizeof( mAddr );
		ret =recvfrom( mSd , rpRecvBuff , rRecvBuffSize , 0 , ( struct sockaddr* )&mAddr , ( socklen_t* )&rAddrLen );

		struct timeval  rAfterTime;
		gettimeofday( &rAfterTime , NULL );

		rTimeDiffSec    = rAfterTime.tv_sec - rBeforeTime.tv_sec;
		rTimeDiffUSec   = rAfterTime.tv_usec - rBeforeTime.tv_usec;
		rTimeDiff       = ( rTimeDiffSec * 1000 ) + ( rTimeDiffUSec / 1000 );

		*rpTimeoutMilliSec  = ( ( rTimer - rTimeDiff ) < 0 ) ? 0 : ( rTimer - rTimeDiff );

		return	ret;
	}

}


int		UDPStream::DefineEvent()
{
	int     ret;
	memset( mpBuffer , 0x00 , mBufferSize );
	memset( &mAddr , 0x00 , sizeof( mAddr ) );
	int		rAddrLen    = sizeof( mAddr );
	ret = recvfrom( mSd , mpBuffer , mBufferSize , 0 , ( struct sockaddr* )&mAddr , ( socklen_t* )&rAddrLen );
	if( ret < 0 )
	{
		mpEvent->SetErrNo( errno );
		mpEvent->SetType( SOCKET_EVENT_ERROR );
		return -1;
	}
	else
	{
		mpEvent->SetRecvData( mpBuffer , ret );
		mpEvent->SetType( SOCKET_EVENT_RECEIVE );
	}
	SetPeerName( inet_ntoa( mAddr.sin_addr ) );

	return  ret;
}

void	UDPStream::SetClientInfo( struct sockaddr_in rAddr )
{
	mAddr	= rAddr;
}

struct sockaddr_in	UDPStream::GetClientInfo()
{
	return	mAddr;
}
