#ifndef		__CLIENT_HPP__
#define		__CLIENT_HPP__

#include	<iostream>
#include	<string>
#include	"Stream.hpp"

using namespace std;

namespace OSS { namespace NETWORK {


class Client : public Stream
{
	private:

	public:
		Client();
		~Client();

		int		Open();
		//string	GetPeerName();

};

}}

#endif
