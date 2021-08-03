#include	"Client.hpp"
using namespace OSS::NETWORK;

Client::Client()
{

}

Client::~Client()
{
}

int		Client::Open()
{
	mPeerName	= mpEvent->GetSocket()->GetPeerName();

	return	0;
}

