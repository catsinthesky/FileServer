#ifndef _LOGGER_
#define _LOGGER_

#include <iostream>
#include <string>
#include <ctime>
#include <mutex>
#include <fstream>

class Logger
{
	public:
		~Logger();
		static Logger* Instance();

	private:
		Logger();
		Logger( const Logger& ){}
		Logger& operator=( const Logger& );

	public:
		void openLogFile( const std::string& logFile );
		void closeLogFile();
		void writeLog( const std::string& msg );
		void writeDebug( const std::string& msg ); 
		void writeError( const std::string& msg );

	private:
		static Logger*		m_pInstance;
		static std::mutex	m_sMutex;
		std::ofstream	    m_fileStream; 
		 
};

#endif
