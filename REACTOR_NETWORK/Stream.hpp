#ifndef		__STREAM_HPP__
#define		__STREAM_HPP__

#include	<string.h>
#include	<iostream>
#include	<string>
#include	"Socket.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class Stream : public Socket
{
	private:
		char*	mpBuffer;
		int		mBufferSize;

	public:
		Stream();
		virtual ~Stream();

		void	SetBuffer( char* rpBuffer , int rSize );

		char*	GetBuffer();
		int		GetBufferSize();
		
		int		Send( char* rpData , int rSize );
		int		Send( string rData );

		int		DefineEvent();

};

}}

#endif
