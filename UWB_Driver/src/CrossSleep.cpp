#include "CrossSleep.h"


void CrossSleep(uint32_t ms)
{
#ifdef __linux__
		usleep(ms * 1000);
#else
		Sleep(ms);
#endif
}
