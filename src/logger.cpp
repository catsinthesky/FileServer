#include "logger.h"

Logger* Logger::m_pInstance = 0;
std::mutex Logger::m_sMutex; 

Logger::Logger()
{

}
Logger::~Logger()
{
	if( m_pInstance != 0 )
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

Logger* Logger::Instance()
{
	// todo, return a static object and replace the methode definition by : static Logger& Instance(); 
	if( !m_pInstance )
	{
		std::lock_guard<std::mutex> guard( m_sMutex);
		if( !m_pInstance )
			m_pInstance = new Logger();
	}
	return m_pInstance;
}

void Logger::openLogFile( const std::string& logFile )
{
	if( !m_fileStream.is_open() )
	{	
		std::lock_guard<std::mutex> guard( m_sMutex);
		m_fileStream.open( logFile.c_str() );  
	}
}

void Logger::closeLogFile()
{
	m_fileStream.close(); 
}


void Logger::writeDebug( const std::string& msg )
{
	writeLog( "DEBUG "+ msg );
}
void Logger::writeError( const std::string& msg )
{
	writeLog( "ERROR "+ msg );
}
void Logger::writeLog( const std::string& msg )
{
	std::lock_guard<std::mutex> guard(m_sMutex);

	time_t t = time(0); 
	struct tm* now = localtime( &t ); 
	
	std::ostream& stream = m_fileStream.is_open() ? m_fileStream : std::cout; 

	stream << "[" << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-" 
		<< (now->tm_mday) << " " << now->tm_hour << ":" << now->tm_min << ":" 
		<< now->tm_sec << "]" << msg << std::endl;   	
}

