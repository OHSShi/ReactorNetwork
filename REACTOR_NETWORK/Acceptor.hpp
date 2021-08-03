#ifndef		__ACCEPTOR_HPP__
#define		__ACCEPTOR_HPP__

#include	<string.h>
#include	<iostream>
#include	<string>
#include	"Socket.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class Acceptor : public Socket
{
	private:
		int		mPortNum;
		struct sockaddr_in	mAddr;
		int		mAddrLen;

	public:
		Acceptor();
		Acceptor( unsigned int rPortNum) : mPortNum( rPortNum ) {};
		virtual ~Acceptor();

		int		Open();
		int		Open( unsigned int rPortNum );
		int		DefineEvent();
		void	SetPortNum( unsigned int rPortNum );
		int		GetPortNum();

};

}}

#endif
