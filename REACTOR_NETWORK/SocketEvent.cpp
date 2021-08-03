#include	"SocketEvent.hpp"
using namespace OSS::NETWORK;
SocketEvent::SocketEvent()
{

}
SocketEvent::~SocketEvent()
{

}
	
void	SocketEvent::SetType( EVENT_TYPE	rType )
{ 
	mType = rType; 
}

EVENT_TYPE	SocketEvent::GetType() 
{ 
	return mType; 
}

void	SocketEvent::SetNewSd( int rSd )
{
	mNewSd	= rSd;
}

int		SocketEvent::GetNewSd()
{
	return	mNewSd;
}

void	SocketEvent::SetSocket( Socket* rpSocket ) 
{
	mpSocket = rpSocket; 
}

Socket*		SocketEvent::GetSocket() 
{ 
	return mpSocket; 
}

void	SocketEvent::SetRecvData( char* rpRecvData , int rRecvBytes )
{
	mRecvBytes	= rRecvBytes;
	//mRecvData.clear();
	//mRecvData.insert( 0 , rpRecvData , rRecvBytes );
	
	mpRecvData	= rpRecvData;
}

int		SocketEvent::GetRecvBytes()
{
	return	mRecvBytes;
}

char*	SocketEvent::GetRecvData()
{
	return mpRecvData;
}

void	SocketEvent::SetErrNo( int rErrNo )
{
	mErrNo	= rErrNo;
}

int		SocketEvent::GetErrNo()
{
	return	mErrNo;
}
