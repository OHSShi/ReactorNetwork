#ifndef __TIMEOUT_EVENT_HANDLER__
#define __TIMEOUT_EVENT_HANDLER__

#include	"SocketEvent.hpp"

namespace OSS { namespace NETWORK {

class TimeoutEventHandler
{
	private:

	protected:

	public:
	    virtual int         TimedOut() = 0;
};

}}

#endif
