#ifndef		__MULTIPLEXER_HPP__
#define		__MULTIPLEXER_HPP__

#include	<iostream>
#include	<string>
#include	<vector>
#include	<map>
#include	<set>
#include	<sys/poll.h>
#include	<sys/time.h>
#include	"Socket.hpp"
#include	"SocketEvent.hpp"
#include	"TimeoutEventHandler.hpp"
#include	"Mutex.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

const int	TCP_MAX_BUFF	= 20480;

typedef	enum
{
	TIMEOUT_NON_BLOCK	,
	TIMEOUT_INFINITE	,
	TIMEOUT_STATIC		,
	TIMEOUT_FALLING		

} TimeoutMode;

class	Multiplexer
{
	private:

		fd_set					mFdSet;

		//static	Multiplexer*	mpInstance;
		vector< Socket* >		mSockets;
		//map< int , Socket* >	mSocketMap;

		SocketEvent				mSharedEvent;

		//struct timeval			mTimer;
		int						mTimer;
		TimeoutMode				mTimeoutMode;

		TimeoutEventHandler*	mpTimeoutEventHandler;

		//char					mSharedBuffer[ TCP_MAX_BUFF ];
		char*					mpSharedBuffer;
		int						mSharedBufferSize;

		bool					mStopFlag;


		Mutex					mMutex;

	public:
		Multiplexer();
		virtual	~Multiplexer();

		//static	Multiplexer*	GetInstance();
		
		int						AddSocket( Socket* rpSocket );
		
		SocketEvent*			GetSharedEvent();

		int						RemoveSocket( Socket* rpSocket );
		void					RemoveSocket();

		char*					GetSharedBuffer();
		void					SetSharedBufferSize( unsigned int rSize );
		int						GetSharedBufferSize();

		int						FdSet();

		void					SetTimeoutEventHandler( TimeoutEventHandler* rpHandler );
		TimeoutEventHandler*	GetTimeoutEventHandler();
		void					SetTimeout( unsigned int rMSec );

		void					SetTimeoutMode( TimeoutMode rTimeoutMode );
		TimeoutMode				GetTimeoutMode();

		int						Run();
		void					Stop();

};

}}

#endif
