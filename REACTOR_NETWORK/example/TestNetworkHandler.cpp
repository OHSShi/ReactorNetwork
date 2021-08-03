#include	"NetworkHandler.h"

void    MySleep( int rSec )
{
	struct timeval tv;
	tv.tv_sec = rSec;
	tv.tv_usec = 0;
	while( 1 )
	{
		int ret = select( 0 , NULL , NULL , NULL , &tv );
		if( ret < 0 )
		{
			if( errno == EINTR )
			{
				continue;
			}
		}

		if( ret == 0 )
		{
			break;
		}
	}
}

int main(int argc , char** argv) 
{
	NetworkHandler networkHandler;

	networkHandler.SetPort(7777);
	networkHandler.Init();
	networkHandler.Run();

	while( 1 ) {
		MySleep( 1 );
	}
}
