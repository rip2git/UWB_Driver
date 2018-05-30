
#include "UserPackFW.h"



void UserPackFW::ToBytes(std::vector <uint8_t> &buffer) const
{
	buffer.clear();
	buffer.reserve(this->DATA_OFFSET + this->TotalSize);
	buffer.push_back(static_cast <uint8_t> (this->FCmd));
	buffer.push_back(static_cast <uint8_t> (this->SCmd));
	buffer.push_back(this->TotalSize);
	buffer.insert(buffer.end(), this->Data.begin(), this->Data.end());
}



void UserPackFW::ToStruct(const std::vector <uint8_t> &buffer)
{
	uint8_t i = 0;
	this->FCmd = static_cast <UserPackFW::FCommand> (buffer[i++]);
	this->SCmd = buffer[i++];
	this->TotalSize = buffer[i++];
	this->Data.assign(buffer.begin() + i, buffer.begin() + i + this->TotalSize);
}



void UserPackFW::SetData(const std::vector <uint8_t> &buffer)
{
	this->TotalSize = buffer.size();
	this->Data.assign(buffer.begin(), buffer.end());

}



void UserPackFW::Reset()
{
	this->FCmd = UserPackFW::FCommand::Service;
	this->SCmd = 0;
	this->TotalSize = 0;
	this->Data.clear();
}



UserPackFW& UserPackFW::operator= (const UserPackFW &pack)
{
	this->FCmd = pack.FCmd;
	this->SCmd = pack.SCmd;
	this->TotalSize = pack.TotalSize;
	this->Data = pack.Data;
	return *this;
}



#ifdef UserPack_DEBUG_MODE
using std::endl;
using std::hex;
using std::dec;
void UserPackFW::Print(std::ostream &os) const
{
	os << "{" << static_cast <int> (this->FCmd) << "}-";
	os << "{" << static_cast <int> (this->SCmd) << "}-";
	os << "{" << static_cast <int> (this->TotalSize) << "}-";
	os << "{";
	for (uint8_t i = 0; i < this->TotalSize; ++i) {
		os << hex << static_cast <int> (this->Data[i]);
		os << dec << " ";
	}
	os << "}" << endl;
}
#endif
