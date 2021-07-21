#include <string>
#include "filePageServer.h"

using namespace std;

int main( int argc, char* argv[] )
{
//./bin/fps $DATAFILE $LOGFILE $LISTENPORT $MAXRECV $MAXCLIENTS $MAXMEMBLOCK $BLOCKSIZE

	if(argc < 8)
	{
		cerr << "Missing Argument" << endl; 
		return -1; 
	}
	
	cout << "dataFile == " << argv[1] <<", logFile == " << argv[2] << " , port ==  " << argv[3] << " , max rcv == " << argv[4] 
	<< " , max clients ==" << argv[5] << " , max meme blocks ==" <<  argv[7] << " , block size ==" <<  argv[6] << endl;  
	
	FilePageServer fps;

	fps.initialize(argv[1], argv[2], ::atoi(argv[3]), ::atoi(argv[4]), ::atoi(argv[5]), ::atoi(argv[6]), ::atoi(argv[7]) );	
	//FilePageServer::initialize( const std::string& dataFile, const std::string& logFile, unsigned int port, unsigned int maxrcv 				 unsigned int maxcnx,unsigned int blocksize, unsigned int maxblocks  )

	fps.run(); 
	
	return 0;
};
