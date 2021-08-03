#include	"Mutex.hpp"

//Mutex*	Mutex::mpInstance = NULL;

Mutex::Mutex()
{
	pthread_mutex_init( &mMutex , NULL );
	pthread_cond_init( &mCond , NULL );
}

Mutex::~Mutex()
{
}

/*
Mutex*	Mutex::GetInstance()
{
	if( mpInstance == NULL )
	{
		mpInstance  = new Mutex;
	}
	return mpInstance;
}
*/
int		Mutex::Lock()
{
	return	pthread_mutex_lock( &mMutex );
}

int		Mutex::UnLock()
{
	return	pthread_mutex_unlock( &mMutex );
}

int		Mutex::CondWait()
{
	return	pthread_cond_wait( &mCond , &mMutex );
}

int		Mutex::CondBroadcast()
{
	return	pthread_cond_broadcast( &mCond );
}

int		Mutex::CondTimedWait( struct timespec* rpTimeSpec )
{
	return	pthread_cond_timedwait( &mCond , &mMutex , rpTimeSpec );
}
