
#include "PrintableUserPack.h"



using std::endl;
using std::hex;
using std::dec;
void PrintableUserPack::Print(std::ostream &os) const
{
	os << "Дрон №";
	os << static_cast <int> (this->SCmd);
	os << " сообщает: ";
	os << "{";
	switch (this->FCmd) {

		case PrintableUserPack::FCommand::Distance:
		{
			for (uint8_t i = 0; i < this->TotalSize-2; ++i)
				os << static_cast <char> (this->Data[i]);
			os << "}; Дистанция: ";
			os << (this->Data[6] + (this->Data[7] << 8));
			os << " см" << endl;
		} break;


		case PrintableUserPack::FCommand::Data:
		{
			for (uint8_t i = 0; i < this->TotalSize; ++i)
				os << static_cast <char> (this->Data[i]) << " ";
			os << "}" << endl;
		} break;


		default:
		{
			for (uint8_t i = 0; i < this->TotalSize; ++i)
				os << static_cast <int> (this->Data[i]) << " ";
			os << "}" << endl;
		} break;

	}
}
