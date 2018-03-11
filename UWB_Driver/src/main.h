
#define _CRT_SECURE_NO_WARNINGS

#ifndef MAIN_H
#define MAIN_H


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <cstdlib>
#include <ctime>

#ifdef __linux__
	#include <errno.h>
	#include <error.h>
	#include <limits.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <termios.h>
	#include <unistd.h>
	#include <signal.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/ioctl.h>
#else
	#include <windows.h>
#endif

#include "CFG.h"
#include "TON.h"
#include "CheckDelegate.h"
#include "ChangeDelegate.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;

typedef unsigned char byte_t;
typedef unsigned short dbyte_t;
typedef unsigned long qbyte_t;

#ifdef __linux__
	typedef qbyte_t DWORD;
	typedef int HANDLE;
#endif

typedef vector <byte_t> ArrayOfBytes;

// -------------------------------------------------------------------------------------
namespace LOG_MODE {
	const int OFF = 0;
	const int CONSOLE = 1;
	const int FILE = 2;
};
// -------------------------------------------------------------------------------------



void CrossSleep(qbyte_t ms)
{
#ifdef __linux__
		usleep(ms * 1000);
#else
		Sleep(ms);
#endif
}



ArrayOfBytes StrToArr(const string str)
{
	ArrayOfBytes array;
	array.resize(str.size());
	for (qbyte_t i = 0; i < str.size(); i++)
		array[i] = str[i];
	return array;
}



string ArrToStr(const ArrayOfBytes &array)
{
	string str;
	str.resize(array.size());
	for (qbyte_t i = 0; i < str.size(); i++)
		str[i] = array[i];
	return str;
}



ArrayOfBytes& operator+=(ArrayOfBytes& left, const ArrayOfBytes& right)
{
    left.insert(left.begin() + left.size(), right.begin(), right.end());
    return left;
}


#endif


