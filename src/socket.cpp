#include "socket.h"

Socket::Socket():m_sock(-1),m_maxConnection(5),m_maxRecv(16)
{
	memset ( &m_addr,0,sizeof( m_addr ) );
}

Socket::~Socket()
{
	if ( is_valid() )
		::close ( m_sock );
}

void Socket::initialize( int port, int maxcnx, int  maxrcv)
{
	m_maxConnection = maxcnx;
	m_maxRecv	= maxrcv;

	Logger::Instance()->writeDebug("Socket::initialize, create"); 
	create(); 

	Logger::Instance()->writeDebug("Socket::initialize, bind port " + std::to_string(port)); 
	bind(port);

	Logger::Instance()->writeDebug("Socket::initialize, listen"); 
	listen();

	Logger::Instance()->writeDebug("Socket::initialize end"); 
}

void Socket::create()
{
	m_sock = socket ( AF_INET, SOCK_STREAM, 0 );

	if ( ! is_valid() )
    	throw FpsException ( "Could not create server socket." );
	
	int on = 1;
	if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
		throw FpsException ( "Could not create server socket." );
}

void Socket::bind ( const int port )
{
	if( ! is_valid() )
		throw FpsException ( "Could not bind to port." );

	m_addr.sin_family	= AF_INET;
	m_addr.sin_addr.s_addr	= INADDR_ANY;
	m_addr.sin_port		= htons( port );

	int bind_return = ::bind( m_sock, (struct sockaddr*) &m_addr,sizeof(m_addr) );

	if ( bind_return == -1 )
		throw FpsException ( "Could not bind to port." );
}


void Socket::listen() const
{
	if ( ! is_valid() )
		throw FpsException ( "Could not listen to socket." );	

	int ret = ::listen( m_sock, m_maxConnection );
	Logger::Instance()->writeDebug("Socket::listen returns " + std::to_string(ret));
	if ( ret == -1 )
		throw FpsException ( "Could not listen to socket." );
}


void Socket::accept( Socket& new_socket ) const
{
	int addr_length = sizeof ( m_addr );
	new_socket.m_sock = ::accept( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

	if ( new_socket.m_sock <= 0 )
		throw FpsException ( "Could not accept socket." );
}

const Socket& Socket::operator << ( const std::string& s ) const
{
	if ( !send( s ) )
		throw FpsException ( "Could not write to socket." );
	Logger::Instance()->writeDebug("Socket:: << " + s);
	return *this;
}


const Socket& Socket::operator >> ( std::string& s ) const
{
	if ( !recv( s ) ) 
		throw FpsException ( "Could not read from socket." );
	Logger::Instance()->writeDebug("Socket:: >> " + s); 
	return *this;
}

bool Socket::send ( const std::string s ) const
{
	if (-1 == ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL ))
		return false;
	  else
		return true;
}


int Socket::recv ( std::string& res ) const
{
	char buf [m_maxRecv+ 1 ];
	res = "";
	memset ( buf, 0, m_maxRecv + 1 );
	
	int status = ::recv ( m_sock, buf, m_maxRecv, 0 );
	
	if( status == -1 )
	{
		Logger::Instance()->writeDebug("Socket::recv ,status -1 ,errno " + std::to_string(errno) );
		return 0;
	}
  	else if ( status == 0 )
	{
		return 0;
	}
	else
	{
		res = buf;  
		return status;
	}
}

void Socket::set_non_blocking ( const bool b )
{
	int opts = fcntl ( m_sock, F_GETFL );

	if ( opts < 0 )
	{
		Logger::Instance()->writeDebug("Socket::set_non_blocking, opts " + std::to_string(opts)); 
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
  	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_sock, F_SETFL, opts );
}

