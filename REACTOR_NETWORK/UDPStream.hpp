#ifndef		__UDP_STREAM_HPP__
#define		__UDP_STREAM_HPP__

#include	<iostream>
#include	<string>
#include	"Socket.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class UDPStream : public Socket
{
	private:

	protected:
		char*					mpBuffer;
		int						mBufferSize;
		struct sockaddr_in		mAddr;

	public:
		UDPStream();
		virtual ~UDPStream();

		void					SetBuffer( char* rpBuffer , int rSize );

		char*					GetBuffer();
		int						GetBufferSize();
		
		int						Send( char* rpData , int rSize );
		int						Send( const string& rData );
		int						Transaction( char* rpData , int rSize , char* rpRecvBuff , int rRecvBuffSize , int rTimeoutMilliSec );
		int						Transaction( char* rpData , int rSize , char* rpRecvBuff , int rRecvBuffSize , int* rpTimeoutMilliSec );

		int						DefineEvent();

		void					SetClientInfo( struct sockaddr_in rAddr );
		struct sockaddr_in		GetClientInfo();


};

}}

#endif
