#include "UserPack.h"



bool UserPack::isCommand(const UserPack::FCommand &Fcmd)
{
	return Fcmd & 0x80;
}



bool UserPack::isResult(const UserPack::FCommand &FCmd)
{
	return FCmd & 0x40;
}



bool UserPack::isCommand(const UserPack::SCommand &Scmd)
{
	return Scmd & 0x80;
}



bool UserPack::isDeviceID(const UserPack::SCommand &Scmd)
{
	return !(Scmd & 0x80);
}


