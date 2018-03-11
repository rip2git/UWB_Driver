#include "COMHandler.h"
#include "CheckSum.h"


union uint16_str {
	uint16 d;
	uint8 b[2];
};



COMHandler::COMHandler()
{
	this->state = COMHandler::STATE::CLOSED;

	COMPort::InitializationStruct init_str;
	init_str.baudRate = 115200;
	init_str.timeOut.Ms = 10;
	init_str.timeOut.nChars = 0;

	char pn[] = "COM1";
	uint8 i;
	for (i = 1; i < 10; ++i) {
		pn[3] = i | 0x30;
		init_str.portName = pn;
		this->port.Initialization(&init_str);
		if (this->port.GetState() == COMPort::STATE::OPENED)
			break;
	}
	if (this->port.GetState() == COMPort::STATE::OPENED) {
		this->portNumber = i;
		this->state = COMHandler::STATE::OPENED;
	}
}



COMHandler::STATE COMHandler::GetState() const
{
	return this->state;
}



COMHandler::RESULT COMHandler::Send(const UserPack *pack) const
{
	uint8 buffer[UserPack::MAX_PACK_BYTE];
	uint8 buf_size = pack->ToBytes(buffer);
	uint16_str crc;
	crc.d = CHECK_SUM_CRC.GetCRC16(buffer, buf_size);
	buffer[buf_size++] = crc.b[0];
	buffer[buf_size++] = crc.b[1];

	int res = this->port.Write(buffer, buf_size);

	return (res == buf_size)? COMHandler::RESULT::SUCCESS : COMHandler::RESULT::ERROR;
}



COMHandler::RESULT COMHandler::Receive(UserPack *pack) const
{
	uint8 buffer[UserPack::MAX_PACK_BYTE];


	pack->Reset();

	int res = this->port.Read(buffer, UserPack::DATA_OFFSET); // read header
	if (res == UserPack::DATA_OFFSET) {
		pack->TotalSize = buffer[2];

		res = this->port.Read(&(buffer[UserPack::DATA_OFFSET]), pack->TotalSize + 2); // read data + 2 crc
		if (res == pack->TotalSize + 2) {
			uint16_str crc;
			uint8 buf_size = UserPack::DATA_OFFSET + pack->TotalSize;
			crc.d = CHECK_SUM_CRC.GetCRC16(buffer, buf_size);
			if (crc.b[0] == buffer[buf_size] && crc.b[1] == buffer[buf_size + 1]) {
				pack->ToStruct(buffer);
				return COMHandler::RESULT::SUCCESS;
			}
		}
	}

	return COMHandler::RESULT::ERROR;
}



uint8 COMHandler::GetPortNumber() const
{
	return this->portNumber;
}


