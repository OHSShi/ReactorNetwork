#include	"Stream.hpp"
using namespace OSS::NETWORK;

Stream::Stream()
{

}

Stream::~Stream()
{

}

void	Stream::SetBuffer( char* rpBuffer , int rSize )
{
	mpBuffer	= rpBuffer;
	mBufferSize	= rSize;
}

char*	Stream::GetBuffer()
{
	return	mpBuffer;
}

int		Stream::GetBufferSize()
{
	return	mBufferSize;
}

int		Stream::Send( char* rpData , int rSize )
{
	int	ret;
	ret	= write( mSd , rpData , rSize );
	if( ret < 0 )
	{
		SetErrNo( errno );
	}
	return	ret;
}

int		Stream::Send( string rData )
{
	return	Send( ( char* )rData.c_str() , rData.size() );
}

int		Stream::DefineEvent()
{
	int		ret;
	memset( mpBuffer , 0x00 , mBufferSize );
	ret	= read( mSd , mpBuffer , mBufferSize );
	if( ret == 0 )
	{
		mpEvent->SetType( SOCKET_EVENT_DISCONNECT );
	}
	else if( ret < 0 )
	{
		mpEvent->SetErrNo( ret );
		mpEvent->SetType( SOCKET_EVENT_ERROR );
	}	
	else
	{
		mpEvent->SetRecvData( mpBuffer , ret );
		mpEvent->SetType( SOCKET_EVENT_RECEIVE );
	}

	return	ret;
}
