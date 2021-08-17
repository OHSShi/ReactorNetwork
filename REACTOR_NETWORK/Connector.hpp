#ifndef __CONNECTOR_HPP__
#define __CONNECTOR_HPP__

#include <stdlib.h>
#include <string>
#include "Stream.hpp"

using namespace std;

namespace OSS { namespace NETWORK {

class Connector : public Stream
{
	private:
		string          mAddress;

	protected:

	public:
		Connector();
		~Connector();

		int             Open();
		int             Open( int rTimeOutSec );

		int             SetAddress(string rAdress);
		string          GetAddress();
};

}}

#endif
