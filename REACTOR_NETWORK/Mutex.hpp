#ifndef __MUTEX_HPP__
#define __MUTEX_HPP__

#include <pthread.h>
#include <iostream>

using namespace std;

class Mutex
{
	private:
		//static Mutex*		mpInstance;

		pthread_mutex_t     mMutex;
		pthread_cond_t		mCond;

	public:
		Mutex();
		~Mutex();

		//static Mutex*		GetInstance();


		int Lock();
		int UnLock();
	
		int	CondWait();
		int	CondBroadcast();
		int	CondTimedWait( struct timespec* rpTimeSpec );


};


#endif
