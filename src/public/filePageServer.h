#ifndef _FPS_
#define _FPS_

#include <string>
#include "logger.h"
#include "socket.h"
#include "fileHandler.h"

using namespace std;

class FilePageServer
{
	public:
		FilePageServer();
		~FilePageServer();
		
		void initialize( const std::string& dataFile, const std::string& logFile, unsigned int port, unsigned int maxrcv,
				 unsigned int maxcnx,unsigned int blocksize, unsigned int maxblocks );
		void run(); 
		void log( const std::string& msg );

	private: 
		int		m_listenPort; 
		unsigned int	m_maxClients; 
		Socket		m_socket; 
		 
};
#endif
