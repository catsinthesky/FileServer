#ifndef _FPSEXCEPTION_
#define _FPSEXCEPTION_

#include <string>

class FpsException
{
	public:
		FpsException( std::string msg ):m_error(msg) {}
		~FpsException(){}

		std::string description() 
		{ 
			return m_error; 
		}

	private:
		std::string m_error;

};

#endif

