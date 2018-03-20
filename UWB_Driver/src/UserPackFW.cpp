#include "UserPackFW.h"


void UserPackFW::ToBytes(std::vector <uint8_t> &buffer) const
{
	buffer.clear();
	buffer.reserve(this->DATA_OFFSET + this->TotalSize);
	buffer.push_back(static_cast <uint8_t> (this->Command));
	buffer.push_back(this->DestinationID);
	buffer.push_back(this->TotalSize);
	buffer.insert(buffer.end(), this->Data.begin(), this->Data.end());
}



void UserPackFW::ToStruct(const std::vector <uint8_t> &buffer)
{
	uint8_t i = 0;
	this->Command = static_cast <UserPackFW::COMMAND> (buffer[i++]);
	this->DestinationID = buffer[i++];
	this->TotalSize = buffer[i++];
	this->Data.assign(buffer.begin() + i, buffer.end());
}



void UserPackFW::SetData(const std::vector <uint8_t> &buffer)
{
	this->TotalSize = buffer.size();
	this->Data.assign(buffer.begin(), buffer.end());

}



void UserPackFW::Reset()
{
	this->Command = UserPackFW::COMMAND::Status;
	this->DestinationID = static_cast <uint8_t> (UserPackFW::STATUS::Reserved);
	this->TotalSize = 0;
	this->Data.clear();
}



#ifdef UserPack_DEBUG_MODE
using std::endl;
using std::hex;
using std::dec;
void UserPackFW::Print(std::ostream &os) const
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
