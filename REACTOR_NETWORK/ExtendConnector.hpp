#ifndef __EXTEND_CONNECTOR_HPP__
#define __EXTEND_CONNECTOR_HPP__

#include <stdlib.h>
#include <string>
#include    <sys/poll.h>
#include    <sys/time.h>
#include "Connector.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

template< class T >
class ExtendConnector : public Connector
{
	private:
		int CalcTimedOut( struct timeval& rBeforeTime , int& rTimer );
	protected:

	public:
		ExtendConnector();
		~ExtendConnector();

		int     Transaction( char* rpData , int rSize , char* rpRecvBuff , int rRecvBuffSize , int rTimeoutMilliSec , T& rAnalyzer );
};

#include	"ExtendConnector.inl"

}}

#endif
