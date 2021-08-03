#ifndef		__UDP_CONNECTOR_HPP__
#define		__UDP_CONNECTOR_HPP__

#include	"UDPStream.hpp"

namespace OSS { namespace NETWORK {

class UDPConnector : public UDPStream
{
	private:
		// xxx.xxx.xxx.xxx:yyyy (ip:port)
		string	mAddress;

	public:
		UDPConnector();
		virtual ~UDPConnector();

		void	SetAddress( string rAddress );
		void	SetAddress( string rIp , int rPort );
		string	GetAddress();

		int		Open();
};

}}

#endif
