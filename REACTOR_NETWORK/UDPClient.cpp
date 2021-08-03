#include	"UDPClient.hpp"
using namespace OSS::NETWORK;

UDPClient::UDPClient()
{

}

UDPClient::~UDPClient()
{

}

int		UDPClient::Open()
{
	mPeerName	= mpEvent->GetSocket()->GetPeerName();
}
