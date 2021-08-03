#include	"Multiplexer.hpp"

using namespace OSS::NETWORK;

//Multiplexer*	Multiplexer::mpInstance	= NULL;

Multiplexer::Multiplexer()
{
	mSharedBufferSize	= TCP_MAX_BUFF;
	mpSharedBuffer		= NULL;
	mTimer				= -1;
	mTimeoutMode		= TIMEOUT_INFINITE;

	mStopFlag			= true;
}

Multiplexer::~Multiplexer()
{
	delete [] mpSharedBuffer;
}
/*
Multiplexer*	Multiplexer::GetInstance()
{
	if( mpInstance == NULL )
	{
		mpInstance	= new Multiplexer;
	}
	return mpInstance;
}
*/

char*	Multiplexer::GetSharedBuffer()
{
	if( mpSharedBuffer == NULL )
	{
		mpSharedBuffer	= new char[ mSharedBufferSize ]; 
		if( mpSharedBuffer == NULL )
		{
			return NULL;
		}
	}

	return mpSharedBuffer;
}

void	Multiplexer::SetSharedBufferSize( unsigned int rSize )
{
	mSharedBufferSize	= rSize;
}

int		Multiplexer::GetSharedBufferSize()
{
	return mSharedBufferSize;
}

int		Multiplexer::AddSocket( Socket* rpSocket )
{
	mSockets.push_back( rpSocket );	

	return	0;
}

int		Multiplexer::RemoveSocket( Socket* rpSocket )
{
	vector< Socket* >::iterator		rIter;

	for( rIter = mSockets.begin() ; rIter != mSockets.end() ; rIter++ )
	{
		if( *rIter == rpSocket )
		{
			mSockets.erase( rIter );
			return	0;
		}
	}

	return	-1;
}

void	Multiplexer::RemoveSocket()
{
	mSockets.clear();
}

SocketEvent*	Multiplexer::GetSharedEvent()
{
	return	&mSharedEvent;
}

void	Multiplexer::SetTimeoutEventHandler( TimeoutEventHandler* rpHandler )
{
	mpTimeoutEventHandler	= rpHandler;
}

TimeoutEventHandler*	Multiplexer::GetTimeoutEventHandler()
{
	return	mpTimeoutEventHandler;
}

void	Multiplexer::SetTimeout( unsigned int rMSec )
{
	mTimer	= rMSec;
}

int		Multiplexer::FdSet()
{
	int		rMaxFd	= -1;
	int		rFd;
	FD_ZERO( &mFdSet );
	for( int i = 0 ; i < mSockets.size() ; i++ )
	{
		rFd	= mSockets[ i ]->GetSd();
		FD_SET( rFd , &mFdSet ); 

		if( rMaxFd < rFd )
		{
			rMaxFd = rFd;
		}
	}

	return	rMaxFd;
}

int		Multiplexer::Run()
{
	int		ret;
	int		rMaxFd;
	int		rEventCount;

	int		rTimer;	//	= mTimer;	
	int		rTimeDiffSec;
	int		rTimeDiffUSec;
	int		rTimeDiff;

	mStopFlag	= false;

	if( mTimeoutMode == TIMEOUT_NON_BLOCK )
	{
		rTimer	= 0;
	}
	else
	{
		rTimer	= mTimer;
	}

	while( 1 )
	{
		if( mStopFlag == true )
		{
			break;
		}


		if( mSockets.size() == 0 )
		{
			cout << "mSockets size 0" << endl;
			return	-1;
		}

		struct pollfd	rPollFd[ mSockets.size() ];
		for( int i = 0 ; i < ( int )mSockets.size() ; i++ )
		{
			int rFd	= mSockets[ i ]->GetSd();
			rPollFd[ i ].fd		= rFd;
			rPollFd[ i ].events	= POLLIN | POLLNVAL;
			rMaxFd	= i;
		}
	
		struct timeval	rBeforeTime;
		gettimeofday( &rBeforeTime , NULL );

		rEventCount	= poll( rPollFd , rMaxFd + 1 , rTimer );
		if( rEventCount < 0 )
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
					rTimer  = mTimer;
					mpTimeoutEventHandler->TimedOut();
				}
				else
				{
					rTimer  = rTimer - ( rTimeDiff );
				}
				continue;
			}
			return	-1;
		}
		else if( rEventCount == 0 )
		{
			rTimer	= mTimer;	
			mpTimeoutEventHandler->TimedOut(); 
			continue;
		}

		vector< Socket* > rSockets = mSockets;

		for( int i = 0 ; i <= rMaxFd ; i++ )
		{
			if( rPollFd[ i ].revents & POLLIN )
			{
				mSharedEvent.SetSocket( rSockets[ i ] );
				rSockets[ i ]->DefineEvent();
				rSockets[ i ]->CallEventHandler();

				if( --rEventCount <= 0 )
				{
					break;
				}
			}
			if( rPollFd[ i ].revents & POLLNVAL )
			{
				mSharedEvent.SetSocket( rSockets[ i ] );
				mSharedEvent.SetType( SOCKET_EVENT_LOST );
				rSockets[ i ]->CallEventHandler();

				if( --rEventCount <= 0 )
				{
					break;
				}
			}

		}

		switch( mTimeoutMode )
		{
			case TIMEOUT_NON_BLOCK :
				rTimer	= 0;
				break;
			case TIMEOUT_STATIC :
				rTimer	= mTimer;
				break;
			case TIMEOUT_FALLING :
			{
				struct timeval	rAfterTime;
				gettimeofday( &rAfterTime , NULL );

				rTimeDiffSec	= rAfterTime.tv_sec - rBeforeTime.tv_sec;
				rTimeDiffUSec	= rAfterTime.tv_usec - rBeforeTime.tv_usec;
				rTimeDiff		= ( rTimeDiffSec * 1000 ) + ( rTimeDiffUSec / 1000 );

				if( rTimeDiff >= rTimer )
				{
					rTimer	= mTimer;
					mpTimeoutEventHandler->TimedOut();
				}
				else
				{
					rTimer	= rTimer - ( rTimeDiff );	
				}
			}
				break;
			default:
				rTimer	= -1;
				break;
		}
	}

	return 0;
}


void	Multiplexer::SetTimeoutMode( TimeoutMode rTimeoutMode )
{
	mTimeoutMode	= rTimeoutMode;
}

TimeoutMode	Multiplexer::GetTimeoutMode()
{
	return	mTimeoutMode;
}

void	Multiplexer::Stop()
{
	mStopFlag	= true;
}

