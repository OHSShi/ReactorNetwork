#ifndef		__UDP_CLIENT_HPP__
#define		__UDP_CLIENT_HPP__

#include	<iostream>
#include	<string>
#include	"Stream.hpp"
#include	"UDPStream.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class UDPClient : public UDPStream
{
	private:

	public:
		UDPClient();
		~UDPClient();

		int		Open();
		//string	GetPeerName();

};

}}

#endif
