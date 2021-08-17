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


	int sd = testConnector.GetSd();
	char rRecvBuff[1024];
	memset( rRecvBuff , 0x00 , sizeof( rRecvBuff ) );

	ret = read( sd , rRecvBuff , sizeof(rRecvBuff) );
	if( ret > 0 )
	{
		string  rReceivedData;
		rReceivedData.insert( 0 , rRecvBuff , ret );
		cout << "read: " << rReceivedData << endl;
	}
	else
	{
		cout << "read Fail!!" << endl;
		return  -1;
	}



	testConnector.Close();
}
