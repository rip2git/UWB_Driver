#ifndef CROSSSLEEP_H_
#define CROSSSLEEP_H_

#include <stdint.h>
#ifdef __linux__
#include <unistd.h>
#else
#include <windows.h>
#endif

extern void CrossSleep(uint32_t ms);


#endif /* CROSSSLEEP_H_ */
