#include	"../Connector.hpp"
#include	<stdio.h>

using namespace OSS::NETWORK;

int main(int argc , char** argv) 
{
	if( argc < 2 )
	{
		printf("Usage: %s <send message>\n", argv[0]);
		exit(-1);
	}


	int	ret;
	Connector testConnector;

	testConnector.SetAddress("127.0.0.1:7777");
	ret = testConnector.Open();
	if( ret < 0 )
	{
		cout << "Connector Open Fail!!" << endl;
		exit(-1);
	}
	testConnector.Send(argv[1]);
	testConnector.Close();
}
