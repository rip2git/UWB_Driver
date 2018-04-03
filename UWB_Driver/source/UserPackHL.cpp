
#include "UserPackHL.h"



void UserPackHL::ToBytes(std::vector <uint8_t> &buffer) const
{
	buffer.clear();
	buffer.reserve(this->DATA_OFFSET + this->TotalSize);
	buffer.push_back(static_cast <uint8_t> (this->Command));
	buffer.push_back(this->DestinationID);
	buffer.push_back(static_cast <uint8_t> (this->TotalSize));
	buffer.push_back(static_cast <uint8_t> (this->TotalSize >> 8));
	buffer.insert(buffer.end(), this->Data.begin(), this->Data.end());
}



void UserPackHL::ToStruct(const std::vector <uint8_t> &buffer)
{
	uint8_t i = 0;
	this->Command = static_cast <UserPackHL::COMMAND> (buffer[i++]);
	this->DestinationID = buffer[i++];
	this->TotalSize = buffer[i++];
	this->TotalSize |= (buffer[i++] << 8);
	this->Data.assign(buffer.begin() + i, buffer.begin() + i + this->TotalSize);
}



void UserPackHL::SetData(const std::vector <uint8_t> &buffer)
{
	this->TotalSize = buffer.size();
	this->Data.assign(buffer.begin(), buffer.end());
}



void UserPackHL::Reset()
{
	this->Command = UserPackHL::COMMAND::Status;
	this->DestinationID = static_cast <uint8_t> (UserPackHL::STATUS::Reserved);
	this->TotalSize = 0;
	this->Data.clear();
}



UserPackHL& UserPackHL::operator= (const UserPackHL &pack)
{
	this->Command = pack.Command;
	this->DestinationID = pack.DestinationID;
	this->TotalSize = pack.TotalSize;
	this->Data = pack.Data;
	return *this;
}



#ifdef UserPack_DEBUG_MODE
using std::endl;
using std::hex;
using std::dec;
void UserPackHL::Print(std::ostream &os) const
{
	os << "{" << static_cast <int> (this->Command) << "}-";
	os << "{" << static_cast <int> (this->DestinationID) << "}-";
	os << "{" << static_cast <int> (this->TotalSize) << "}-";
	os << "{";
	for (uint8_t i = 0; i < this->TotalSize; ++i)
		os << static_cast <char> (this->Data[i]);
	os << "}";
}
#endif
