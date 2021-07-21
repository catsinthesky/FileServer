#include "fileHandler.h"

FileHandler* FileHandler::m_pInstance = 0;
std::mutex FileHandler::m_sMutex; 

FileHandler::FileHandler()
{
	m_fileName	= "defaultdata";
	m_blockSize	= 0;
	m_maxBlocks	= 0;
}

FileHandler::~FileHandler()
{
}

FileHandler* FileHandler::Instance()
{
	if( !m_pInstance )
	{
		std::lock_guard<std::mutex> guard(m_sMutex);
		if( !m_pInstance )
			m_pInstance = new FileHandler();
	}
	return m_pInstance;
}

void FileHandler::initialize(const std::string& filename, int blocksize, int maxblocks)
{
	Logger::Instance()->writeDebug("FileHandler::initialize , filename :"+ filename + ", blocksize :" 
				+ std::to_string(blocksize) + ", maxblocks : " + std::to_string(maxblocks) ); 
	
	m_fileName	= filename;
	m_blockSize	= blocksize;
	m_maxBlocks	= maxblocks;
}

void FileHandler::openFile()
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	Logger::Instance()->writeDebug("FileHandler::openFile");
	m_pDataFile = fopen(m_fileName.c_str(), "r");

	if (m_pDataFile == NULL)
		throw FpsException("FileHandler::openFileopening , error file "+ m_fileName);
}

void FileHandler::closeFile()
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	Logger::Instance()->writeDebug("FileHandler::closeFile");
	fclose(m_pDataFile); 
}

void FileHandler::getBlock (const std::string& strBlocknumber, std::string& result )
{
	std::lock_guard<std::mutex> guard(m_sMutex);

	result = "--no content--";

 	// extracting block number
	std::stringstream ss; 
	ss << strBlocknumber; 
	
	int blocknumber; 
	ss >> blocknumber; 

	if(blocknumber <= 0)
	{
		Logger::Instance()->writeDebug( "FileHandler::getBlock not a good block number " + std::to_string(blocknumber) );
		return;
	}

	// search for the block in the memory cash
	std::map<int,std::string>::iterator it = m_requestedBlocks.find(blocknumber);
	
	if(it != m_requestedBlocks.end())
	{
		result = it->second; 
		Logger::Instance()->writeDebug("FileHandler::getBlock result from cashmemory : " + result); 
	}
	else
	{
		// seek for the value in the data file
		int tmp = fseek( m_pDataFile, (m_blockSize*blocknumber), SEEK_SET);
		
		if( tmp == 0 )
		{
			char res[m_blockSize+1]; 
			if ( fgets(res, m_blockSize+1, m_pDataFile) != NULL )
				result = std::string(res);
			Logger::Instance()->writeDebug("FileHandler::getBlock result from file : " + result); 
		}
		else
		{
			throw FpsException( "FileHandler::getBlock Error fseek" );
		}
	} 
		
	// insert/delete new/oldest blocks 	
	processCashblocks(blocknumber, result); 	
}

void FileHandler::processCashblocks( int blocknumber, const std::string& block )
{
	std::lock_guard<std::mutex> guard(m_sMutex);
	Logger::Instance()->writeDebug("FileHandler::processCashblocks, blocknumber :" + std::to_string(blocknumber) + ", value : " + block );
	std::queue<int> tmp; 
	while(!m_requestedQueue.empty())
	{
		if(blocknumber != m_requestedQueue.front())
			tmp.push(blocknumber); 
		m_requestedQueue.pop();
	}
	m_requestedQueue.swap(tmp); 
	
	if(m_maxBlocks <= m_requestedQueue.size())
	{ 
		m_requestedBlocks.erase(m_requestedQueue.front());
		m_requestedQueue.pop();  
	}
	m_requestedQueue.push(blocknumber);

	m_requestedBlocks.insert( make_pair(blocknumber, block) ); 
}

void FileHandler::getCashedValues()
{
	Logger::Instance()->writeDebug("FileHandler::getCashedValues, memory contains :");
	for (auto it = m_requestedBlocks.begin(); it != m_requestedBlocks.end(); ++it)
		Logger::Instance()->writeDebug("block number {"+ std::to_string(it->first) + "} value {"+ it->second +"}");

	Logger::Instance()->writeDebug("FileHandle::getCashedValues, most requested :");
	while( !m_requestedQueue.empty())
	    Logger::Instance()->writeDebug(std::to_string(m_requestedQueue.front()));
}
// setters 
void FileHandler::setBlockSize( int bs )
{
	m_blockSize = bs; 
}
void FileHandler::setMaxBlock( int max )
{
	m_maxBlocks = max;
}

