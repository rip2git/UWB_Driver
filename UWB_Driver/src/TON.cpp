#include "TON.h"

typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::duration<std::chrono::milliseconds> HighResTime;
#define HighResTimeGet() std::chrono::high_resolution_clock::now().time_since_epoch()
#define HighResTimeToMilliseconds(x) std::chrono::duration_cast<std::chrono::milliseconds>(x)



TON::TON()
{
	this->_Q = false;
	this->_IN = false;
}



void TON::start(long long int _PT, bool _IN)
{
	this->_PT = milliseconds(_PT);
	if (!this->_IN && _IN) {
		this->_stT = HighResTimeToMilliseconds(HighResTimeGet());
	}
	this->_IN = _IN;
}



void TON::reset()
{
	this->_IN = false;
	this->_Q = false;
}



long long int TON::since()
{
	this->_ET = HighResTimeToMilliseconds(HighResTimeGet());
	this->_ET -= this->_stT;
	return HighResTimeToMilliseconds(this->_ET).count();
}



bool TON::check()
{
	this->since();
	if (this->_ET >= this->_PT && this->_IN)
		this->_Q = true;
	return this->_Q;
}

/*
#ifdef LINUX_RELEASE
TON::TON()
{
	this->_Q = false;
	this->_IN = false;
}



void TON::start(unsigned long long _PT, bool _IN)
{
	this->_PT.tv_nsec = (_PT % 1000) * 1000000;
	this->_PT.tv_sec = _PT / 1000;
	this->_IN = _IN;
	if (this->_IN) {
		clock_gettime(CLOCK_MONOTONIC, &this->_stT);
	}
}



void TON::reset()
{
	this->_IN = false;
	this->_Q = false;
}



unsigned long long TON::since()
{
	clock_gettime(CLOCK_MONOTONIC, &this->_ET);
	unsigned long long _ET_ms = this->_ET.tv_sec * 1000 + this->_ET.tv_nsec / 1000000;
	unsigned long long _stT_ms = this->_stT.tv_sec * 1000 + this->_stT.tv_nsec / 1000000;
	_ET_ms -= _stT_ms;

	this->_ET.tv_nsec = (_ET_ms % 1000) * 1000000;
	this->_ET.tv_sec = _ET_ms / 1000;
	return _ET_ms;
}



bool TON::check()
{
	this->since();
	unsigned long long _ET_ms = this->_ET.tv_sec * 1000 + this->_ET.tv_nsec / 1000000;
	unsigned long long _PT_ms = this->_PT.tv_sec * 1000 + this->_PT.tv_nsec / 1000000;
	if (_ET_ms >= _PT_ms && this->_IN)
		this->_Q = true;
	return this->_Q;
}



#else
TON::TON()
{
	this->_Q = false;
	this->_IN = false;
	this->_ET = 0;
	this->_PT = 0;
	this->_stT = 0;
}



void TON::start(ULONGLONG _PT, bool _IN)
{
	this->_PT = _PT;
	this->_IN = _IN;
	if (this->_IN) {
		this->_stT = GetTickCount64();
	}
}



void TON::reset()
{
	this->_IN = false;
	this->_Q = false;
}



ULONGLONG TON::since()
{
	this->_ET = GetTickCount64();
	return this->_ET -= this->_stT;
}



bool TON::check()
{
	this->since();
	if (this->_ET >= this->_PT && this->_IN)
		this->_Q = true;
	return this->_Q;
}
#endif
*/
