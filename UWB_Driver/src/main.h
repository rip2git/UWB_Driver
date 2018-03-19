
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
#include <thread>
#include <mutex>
#include <stdint.h>

#ifdef __linux__
	#include <unistd.h>
#else
	#include <windows.h>
#endif

#include "TON.h"
#include "COMHandler.h"
#include "CheckDelegate.h"
#include "ChangeDelegate.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;

typedef vector <uint8_t> ArrayOfBytes;

// -------------------------------------------------------------------------------------
namespace LOG_MODE {
	const int OFF = 0;
	const int CONSOLE = 1;
	const int FILE = 2;
};
// -------------------------------------------------------------------------------------



void CrossSleep(uint32_t ms)
{
#ifdef __linux__
		usleep(ms * 1000);
#else
		Sleep(ms);
#endif
}



#endif


