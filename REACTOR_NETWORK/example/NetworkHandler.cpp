#include	"NetworkHandler.h"
#include	<stdio.h>
#include	<pthread.h>

static string      ConvertHexToString( char* rInput , int rLen )
{
	string rValue = "";

	for( int i = 0 ; i < rLen ; i++ )
	{
		char tmp1 = ( '0' + ( ( rInput[i] & 0xf0 )  >> 4 ) );
		char tmp2 = ( '0' + ( rInput[i] & 0x0f ) );

		if( tmp1 > '9' )
			tmp1 += 7;
		if( tmp2 > '9' )
			tmp2 += 7;

		rValue += tmp1;
		rValue += tmp2;
	}

	return rValue;
}

static void*    WorkingThreadStart( void* rpArg )
{
	NetworkHandler* rpThis    = ( NetworkHandler* )rpArg;
	rpThis->WorkingThread();
}

NetworkHandler::NetworkHandler()
{
	mTimeOutSec	= 5;
}

NetworkHandler::~NetworkHandler()
{
}

int		NetworkHandler::ClientAccepted( SocketEvent* rpEvent )
{
	int	ret;
	int	rSd	= rpEvent->GetNewSd();

	printf( "Client Accepted. IP[%s]\n" , rpEvent->GetSocket()->GetPeerName().c_str() );

    if (rpEvent->GetSocket() == &mAcceptor)
    {
	    printf( "TCP Accepted\n");
        Client*	rpClient	= new Client;
        rpClient->SetSd( rSd );

        ret = rpClient->TransNonBlock();
        if( ret < 0 )
        {
            printf( "ERR: Client Translating Non-Block Fail!! fd(%d)" , rpClient->GetSd() );
            rpClient->Close();
            delete rpClient;
            return  -1;
        }

        rpClient->SetPeerName( rpEvent->GetSocket()->GetPeerName() );
        rpClient->SetEvent( mpSharedEvent );
        rpClient->SetEventHandler( this );
        rpClient->SetBuffer( mpSharedBuffer , mSharedBufferSize );

        mpMultiplexer->AddSocket( rpClient );
    }
    else {
        printf("unknown socket\n");
    }
	return	0;
}

int		NetworkHandler::DataReceived( SocketEvent* rpEvent )
{
	printf( "Size(%d) " , rpEvent->GetRecvBytes() );

	string	rRecvData;
	rRecvData.insert( 0 , rpEvent->GetRecvData() , rpEvent->GetRecvBytes() );

	printf( "(%s)\n" , rRecvData.c_str() );

    if (rpEvent->GetSocket() == &mAcceptor)
    {
        Client*	rpClient	= ( Client* )rpEvent->GetSocket();
        rpClient->Send( "APPLE" );
    }
    else if( rpEvent->GetSocket() == &mUdpAcceptor )
    {
        mUdpAcceptor.Send("APPLE");
    }

	return	0;
}

int		NetworkHandler::Disconnected( SocketEvent* rpEvent )
{
	Client*	rpClient	= ( Client* )rpEvent->GetSocket();
	printf( "Client Disconnected... fd(%d) IP[%s]\n" , rpClient->GetSd(), rpEvent->GetSocket()->GetPeerName().c_str() );
	
	mpMultiplexer->RemoveSocket( rpClient );
	rpClient->Close();
	delete rpClient;

	return	0;
}

int		NetworkHandler::ConnectionLost( SocketEvent* rpEvent )
{
	Client*	rpClient	= ( Client* )rpEvent->GetSocket();
	printf( "Connection Lost... fd(%d)\n" , rpClient->GetSd() );
	
	mpMultiplexer->RemoveSocket( rpClient );
	rpClient->Close();
	delete rpClient;

	return	0;
}

int		NetworkHandler::TimedOut()
{	
	cout << "TimedOut..." << endl;
	return	0;
}

int		NetworkHandler::ErrorHandling( SocketEvent* rpEvent )
{
	printf( "ERROR: %d\n", rpEvent->GetErrNo() );

	return	0;
}


int		NetworkHandler::Run()
{
	int ret;

	pthread_attr_t  attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED );

	pthread_create( &mThreadId , &attr , WorkingThreadStart, ( void* )this );
	pthread_attr_destroy(&attr);

	return	0;
}

void	NetworkHandler::SetPort( int rPort )
{
	mPortNum	= rPort;
}

int		NetworkHandler::Init()
{
	int		ret;

	//Multiplexer & Acceptor Setting
	mpMultiplexer       = new Multiplexer;
	mpSharedEvent       = mpMultiplexer->GetSharedEvent();
	mpSharedBuffer      = mpMultiplexer->GetSharedBuffer();
	mSharedBufferSize   = mpMultiplexer->GetSharedBufferSize();

	mpMultiplexer->SetTimeout( mTimeOutSec * 1000 );
	mpMultiplexer->SetTimeoutMode( TIMEOUT_FALLING );
	mpMultiplexer->SetTimeoutEventHandler( this );

	mAcceptor.SetEventHandler( this );
	mAcceptor.SetEvent( mpSharedEvent );
	mAcceptor.SetPortNum( mPortNum );
	ret = mAcceptor.Open();
	if( ret < 0 )
	{
		printf( "ERR: Acceptor Can't Open. Err Str(%s)" , mAcceptor.GetErrorString().c_str() );
		return  -1;
	}
	ret = mAcceptor.TransNonBlock();
	if( ret < 0 )
	{
		mAcceptor.Close();
		printf( "ERR: Acceptor.TransNonBlock() Fail" );
		return  -1;
	}

	mpMultiplexer->AddSocket( &mAcceptor );

	mUdpAcceptor.SetEventHandler( this );
	mUdpAcceptor.SetEvent( mpSharedEvent );
	mUdpAcceptor.SetPortNum( 7777 );
    mUdpAcceptor.SetBuffer( mpSharedBuffer , mSharedBufferSize );
	ret = mUdpAcceptor.Open();
	if( ret < 0 )
	{
		printf( "ERR: UDPAcceptor Can't Open. Err Str(%s)" , mUdpAcceptor.GetErrorString().c_str() );
		return  -1;
	}
	ret = mUdpAcceptor.TransNonBlock();
	if( ret < 0 )
	{
		mUdpAcceptor.Close();
		printf( "ERR: Acceptor.TransNonBlock() Fail" );
		return  -1;
	}

	mpMultiplexer->AddSocket( &mUdpAcceptor );

	return	0;
}

void	NetworkHandler::WorkingThread()
{
	mpMultiplexer->Run();
}
