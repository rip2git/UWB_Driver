
#include "COMHandler.h"
#include "CheckSum.h"
#include "CFG.h"
#include "IniFiles.h"
#include <stdlib.h>



union uint16_str {
	uint16_t d;
	uint8_t b[2];
};



/*! ----------------------------------------------------------------------------------------
 * @brief: Object for using
 * -----------------------------------------------------------------------------------------
 * */
static const CheckSum checkSum;



COMHandler::COMHandler()
{
	this->Initialization();
}



void COMHandler::Initialization()
{
	this->state = COMHandler::STATE::CLOSED;

	try {
		std::string pn;
		std::map <std::string, std::string> ucnf;
		COMPort::InitializationStruct init_str;
		IniFile ini(CFG::FILE_NAME);

		if (ini.isOpened())
			ini.Parse();
		else
			throw std::exception();

		ucnf = ini.GetSection(CFG::COM::SECTION);

		// prevents segmentation fault from stoi
		if ( ucnf.find(CFG::COM::COM_NAME) 	== ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::COM::BAUD_RATE) == ucnf.end() ) throw std::exception();

		pn = ucnf.find(CFG::COM::COM_NAME)->second;
		init_str.portName = &( pn[0] );
		init_str.baudRate = std::stoi( ucnf.find(CFG::COM::BAUD_RATE)->second );
		init_str.timeOut.Ms = 50;
		init_str.timeOut.nChars = 0;

		this->port.Initialization(init_str);

		if (this->port.GetState() == COMPort::STATE::OPENED) {
			this->portNumber = (pn.size() == 4)? atoi( pn.substr(3,1).c_str() ) : atoi( pn.substr(3,2).c_str() );
			this->state = COMHandler::STATE::OPENED;
		}

	} catch (std::exception &e) {
		;
	}
}



COMHandler::STATE COMHandler::GetState() const
{
	return this->state;
}



COMHandler::RESULT COMHandler::Send(const UserPackFW &pack) const
{
	std::vector <uint8_t> buffer;
	pack.ToBytes(buffer);
	uint16_str crc;
	crc.d = checkSum.GetCRC16( &(buffer[0]), buffer.size() );
	buffer.push_back(crc.b[0]);
	buffer.push_back(crc.b[1]);

	int res = this->port.Write( &(buffer[0]), buffer.size() );

	return (res == static_cast <int> (buffer.size()))?
			COMHandler::RESULT::SUCCESS : COMHandler::RESULT::ERROR;
}



COMHandler::RESULT COMHandler::Receive(UserPackFW &pack) const
{
	std::vector <uint8_t> buffer;
	buffer.resize(UserPackFW::DATA_OFFSET);

	pack.Reset();

	int res = this->port.Read( &(buffer[0]), UserPackFW::DATA_OFFSET ); // read header
	if (res == UserPackFW::DATA_OFFSET) {
		pack.TotalSize = buffer[2];
		buffer.resize(UserPackFW::DATA_OFFSET + pack.TotalSize + 2); // saves content

		res = this->port.Read( &(buffer[UserPackFW::DATA_OFFSET]), pack.TotalSize + 2 ); // read data + 2 crc
		if (res == pack.TotalSize + 2) {
			uint16_str crc;
			uint8_t buf_size = UserPackFW::DATA_OFFSET + pack.TotalSize;
			crc.d = checkSum.GetCRC16( &(buffer[0]), buf_size );
			if (crc.b[0] == buffer[buf_size] && crc.b[1] == buffer[buf_size + 1]) {
				pack.ToStruct(buffer);
				return COMHandler::RESULT::SUCCESS;
			}
		}
	}

	return COMHandler::RESULT::ERROR;
}



uint8_t COMHandler::GetPortNumber() const
{
	return this->portNumber;
}



#ifdef COMHandler_DEBUG
COMPort& COMHandler::GetPortDirectly() {
	return this->port;
}
#endif


