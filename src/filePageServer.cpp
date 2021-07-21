#include "filePageServer.h"

FilePageServer::FilePageServer()
{
}

FilePageServer::~FilePageServer()
{ 
}

void FilePageServer::initialize( const std::string& dataFile, const std::string& logFile, unsigned int port, unsigned int maxrcv,
				 unsigned int maxcnx,unsigned int blocksize, unsigned int maxblocks )
{
	// create Logger
	Logger::Instance()->openLogFile(logFile); 
	log("Initialization ..."); 

	// open read-only the data file
	FileHandler::Instance()->initialize(dataFile, blocksize, maxblocks); 
	FileHandler::Instance()->openFile(); 

	// initializing socket
	m_socket.initialize( port, maxcnx, maxrcv);
}

void FilePageServer::run()
{
	//TO BE COMPLETED ; runnign the server and getting blocks
   	while ( true )
	{
	  Socket new_sock;
	  m_socket.accept( new_sock );
      log ("New socket creation");
	  try
	  {
	  	while ( true )
		{
			std::string blockNumber, data;
			// request
			new_sock >> blockNumber;
		  	// getting data from file handler	
			FileHandler::Instance()->getBlock( blockNumber, data );
			// response
			new_sock << "response :" + data + '\n';
		}
	  }
	  catch ( FpsException& e) 
	  {
		 log( e.description() ); 
	  }
	}
	FileHandler::Instance()->getCashedValues();
} 
void FilePageServer::log( const std::string& msg )
{
	Logger::Instance()->writeLog( msg );
}
