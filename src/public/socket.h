// Definition of the Socket class

#ifndef _SOCKET_
#define _SOCKET_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "fpsException.h"
#include "logger.h"

class Socket
{
	public:
		Socket();
		~Socket();

		// Server initialization
		void initialize(int port, int maxcnx, int  maxrcv); 
		void accept ( Socket& ) const;
		const Socket& operator << ( const std::string& ) const;
		const Socket& operator >> ( std::string& ) const;

	private:
		void create();
		void bind ( const int port );
		void listen() const;
	
		bool send ( const std::string ) const;
		int recv ( std::string& ) const;

		void set_non_blocking ( const bool );

		bool is_valid() const { return m_sock != -1; }

	private:
		int	m_sock;
  		sockaddr_in	m_addr;
		int	m_maxConnection;
		int	m_maxRecv;
};
#endif

