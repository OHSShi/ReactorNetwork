#ifndef		__UDP_ACCEPTOR_HPP__
#define		__UDP_ACCEPTOR_HPP__

#include	<iostream>
#include	<string>
#include	"Socket.hpp"
#include	"UDPStream.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class UDPAcceptor : public UDPStream
{
	private:
		int		mPortNum;
		struct sockaddr_in	mAddr;
		int		mAddrLen;

	public:
		UDPAcceptor();
		UDPAcceptor( unsigned int rPortNum) : mPortNum( rPortNum ) {};
		virtual ~UDPAcceptor();

		int		Open();
		int		Open( unsigned int rPortNum );
		void	SetPortNum( unsigned int rPortNum );
		int		GetPortNum();

};

}}

#endif
