#ifndef _FILE_HANDLER_
#define _FILE_HANDLER_

#include <string>
#include <mutex>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <cstdio>
#include <stdio.h>

#include "fpsException.h"
#include "logger.h"

class FileHandler
{
	public:
		~FileHandler();
		static FileHandler* Instance();

	private:
		FileHandler();
		FileHandler( const FileHandler& );
		FileHandler& operator=( const FileHandler& );
		void processCashblocks( int blocknumber, const std::string& block);

	public:
		void initialize(const std::string& filename, int blocksize, int maxblocks); 
		void openFile();
		void closeFile();
		void setBlockSize(int); 
		void setMaxBlock(int);
		void getBlock( const std::string& blocknumber, std::string& result ); 
		void getCashedValues();

	private:
		static FileHandler*	m_pInstance;
		static std::mutex	m_sMutex;
		FILE*       		m_pDataFile; 
		std::string			m_fileName;
		int		        	m_blockSize; 
		unsigned int		m_maxBlocks; 
		std::queue<int>		m_requestedQueue; 
		std::map<int,std::string>	m_requestedBlocks;
};
#endif
