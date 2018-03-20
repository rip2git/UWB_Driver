
#define _CRT_SECURE_NO_WARNINGS

#ifndef MAIN_H
#define MAIN_H


#ifdef __linux__
	#include <unistd.h>
#else
	#include <windows.h>
#endif


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <cstdint>


#include "CrossSleep.h"
#include "TON.h"
#include "COMHandler.h"
#include "CheckDelegate.h"
#include "ChangeDelegate.h"
#include "IniFiles.h"
#include "CFG.h"
#include "Logger.h"
#include "UserInterface.h"


using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;



#endif


