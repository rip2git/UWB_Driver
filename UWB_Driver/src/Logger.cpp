#include <iostream>
#include "Logger.h"


Logger::~Logger()
{
	if (this->is_open())
		this->close();
}



void Logger::toNull()
{
	this->basic_ios<char>::rdbuf( 0 );
}



void Logger::toConsole()
{
	this->basic_ios<char>::rdbuf( std::cout.rdbuf() );
}



void Logger::toFile()
{
	const size_t buf_size = 40;
	size_t iter = 0;
	char buf[buf_size];
	std::string format = "%Y_%b_%d_%H:%M:%S_log";
	time_t seconds = time(NULL);
	tm *timeinfo = localtime(&seconds);
	strftime(buf, buf_size, format.c_str(), timeinfo);
	std::string filename;
	filename.reserve(buf_size);
	while ( iter < buf_size && buf[iter] != '\0' ) {
		filename.push_back(buf[iter]);
		iter++;
	}
	filename.resize(iter);
	filename += ".txt";
	this->open(filename.c_str(), std::ios_base::out);
}



