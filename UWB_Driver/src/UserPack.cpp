#include "UserPack.h"


uint8_t UserPack::ToBytes(uint8_t *buffer) const
{
	uint8_t i = 0;
	buffer[i++] = static_cast <uint8_t> (this->Command);
	buffer[i++] = this->DestinationID;
	buffer[i++] = this->TotalSize;
	for (uint8_t j = 0; j < this->TotalSize; ++j)
		buffer[i++] = this->Data[j];
	return i;
}



void UserPack::ToStruct(const uint8_t *buffer)
{
	uint8_t i = 0;
	this->Command = static_cast <UserPack::COMMAND> (buffer[i++]);
	this->DestinationID = buffer[i++];
	this->TotalSize = buffer[i++];
	for (uint8_t j = 0; j < this->TotalSize; ++j)
		this->Data[j] = buffer[i++];
}



void UserPack::SetData(const uint8_t *buffer)
{
	for (uint8_t i = 0; i < this->TotalSize; ++i)
		this->Data[i] = buffer[i];

}



void UserPack::Reset()
{
	this->Command = UserPack::COMMAND::Status;
	this->DestinationID = static_cast <uint8_t> (UserPack::STATUS::Reserved);
	this->TotalSize = 0;
}



#ifdef UserPack_DEBUG_MODE
#include <iostream>
using std::cout;
using std::endl;
using std::hex;
using std::dec;
void UserPack::Print() const
{
	cout << "{" << static_cast <int> (this->Command) << "}-";
	cout << "{" << static_cast <int> (this->DestinationID) << "}-";
	cout << "{" << static_cast <int> (this->TotalSize) << "}-";
	cout << "{";
	if (this->TotalSize > 2) {
		for (uint8_t i = 0; i < this->TotalSize - 2; ++i)
			cout << static_cast <char> (this->Data[i]);
		cout << "}";
	}
}
#endif
