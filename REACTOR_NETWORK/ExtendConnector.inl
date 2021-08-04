template < class T >
ExtendConnector< T >::ExtendConnector()
{
}

template < class T >
ExtendConnector< T >::~ExtendConnector()
{

}

template < class T >
int		ExtendConnector< T >::CalcTimedOut( struct timeval& rBeforeTime , int& rTimer )
{
	struct timeval  rAfterTime;
	gettimeofday( &rAfterTime , NULL );

	int rTimeDiffSec    = rAfterTime.tv_sec - rBeforeTime.tv_sec;
	int rTimeDiffUSec   = rAfterTime.tv_usec - rBeforeTime.tv_usec;
	int rTimeDiff       = ( rTimeDiffSec * 1000 ) + ( rTimeDiffUSec / 1000 );

	if( rTimeDiff >= rTimer )
	{
		return  0;  // Timeout.
	}
	else
	{
		rTimer  = rTimer - ( rTimeDiff );
	}

	return  rTimer;
}



template < class T >
int		ExtendConnector< T >::Transaction( char* rpData , int rSize , char* rpRecvBuff , int rRecvBuffSize , int rTimeoutMilliSec , T& rAnalyzer )
{
    int ret = write( mSd , rpData , rSize );
    if( ret < 0 )
    {
		Socket::SetErrorString( "write Fail" );
        return  ret;
    }

    struct pollfd   rPollFd;
    int rTimer   = rTimeoutMilliSec;
    //int rTimeDiff;

    rPollFd.fd      = mSd;
    rPollFd.events  = POLLIN | POLLNVAL;

    int     rTotalLen   = 0;


    while( 1 )
    {
        struct timeval rBeforeTime;
        gettimeofday( &rBeforeTime , NULL );

        ret = poll( &rPollFd , 1 , rTimer );
        if( ret < 0 )
        {
            if( errno == EINTR )
            {
                rTimer  = CalcTimedOut( rBeforeTime , rTimer );
                if( rTimer == 0 )
                {
                    return  0;
                }
                continue;
            }
			Socket::SetErrorString( "poll fail" );
            return  -1;
        }
        else if( ret == 0 ) // Timeout.
        {
            return  0;
        }

        if( rPollFd.revents & POLLNVAL )
        {
			Socket::SetErrorString( "POLLNVAL" );
            return  -1;
        }

        ret = read( mSd , rpRecvBuff , rRecvBuffSize );
        if( read > 0 )
        {
            rTotalLen += ret;
        }
        else 
        {
			Socket::SetErrorString( "read fail" );
            return  -1;
        }
		
    	string  rReceivedData;
        rReceivedData.insert( 0 , rpRecvBuff , ret );

        ret = rAnalyzer( rReceivedData );
        if( ret == MSG_ANALYZE_SUCCESS )
        {
            break;
        }
        else if( ret == MSG_ANALYZE_NOT_CLOSE )
        {
            rTimer  = CalcTimedOut( rBeforeTime , rTimer );
            continue;
        }
        else  // if ( ret == MSG_ANALYZE_FAIL || ret == MSG_ANALYZE_SPARE )
        {
			Socket::SetErrorString( "Analyze Fail" );
            return  -1;
        }
    }

    return  rTotalLen;
}


