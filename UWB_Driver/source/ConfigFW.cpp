#include "ConfigFW.h"
#include "CFG.h"
#include "IniFiles.h"



void ConfigFW::ToUserPackFW(UserPackFW &pack) const
{
	uint8_t i = 0;
	pack.FCmd = UserPack::Command::SystemConfig;
	pack.SCmd = 0;
	pack.TotalSize = this->configSize;
	pack.Data.resize(pack.TotalSize);
	//
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.DebugMode);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.DeviceID);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.DeviceID >> 8);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.PAN_ID);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.PAN_ID >> 8);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.nDevices);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.PollingPeriod);
	pack.Data[i++] = static_cast <uint8_t> (this->SW1000.PollingPeriod >> 8);
	//
	pack.Data[i++] = static_cast <uint8_t> (this->Token.TimeSlotDurationMs);
	//
	pack.Data[i++] = static_cast <uint8_t> (this->Routing.MinSignalLevel);
	//
	pack.Data[i++] = static_cast <uint8_t> (this->Ranging.RespondingDelay);
	pack.Data[i++] = static_cast <uint8_t> (this->Ranging.RespondingDelay >> 8);
	pack.Data[i++] = static_cast <uint8_t> (this->Ranging.FinalDelay);
	pack.Data[i++] = static_cast <uint8_t> (this->Ranging.FinalDelay >> 8);
}



ConfigFW::ConfigFW()
{
	this->state = ConfigFW::STATE::ERROR;
	try {
		std::map <std::string, std::string> ucnf;
		IniFile ini(CFG::FILE_NAME);

		if (ini.isOpened())
			ini.Parse();
		else
			throw std::exception();

		ucnf = ini.GetSection(CFG::FIRMWARE::SECTION);

		// prevents segmentation fault from stoi
		if ( ucnf.find(CFG::FIRMWARE::DEBUG_MODE) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::DEVICE_ID) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::PAN_ID) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::NUM_DEVICES) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::POLL_PERIOD) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::TOKEN_TSL_DUR) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::MIN_SIG_LVL) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::RESP_DELAY) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::FIRMWARE::FINAL_DELAY) == ucnf.end() ) throw std::exception();

		this->SW1000.DebugMode = std::stoi( ucnf.find(CFG::FIRMWARE::DEBUG_MODE)->second );
		this->SW1000.DeviceID = std::stoi( ucnf.find(CFG::FIRMWARE::DEVICE_ID)->second );
		this->SW1000.PAN_ID = std::stoi( ucnf.find(CFG::FIRMWARE::PAN_ID)->second );
		this->SW1000.nDevices =	std::stoi( ucnf.find(CFG::FIRMWARE::NUM_DEVICES)->second );
		this->SW1000.PollingPeriod = std::stoi( ucnf.find(CFG::FIRMWARE::POLL_PERIOD)->second );
		this->Token.TimeSlotDurationMs = std::stoi( ucnf.find(CFG::FIRMWARE::TOKEN_TSL_DUR)->second );
		this->Routing.MinSignalLevel = std::stoi( ucnf.find(CFG::FIRMWARE::MIN_SIG_LVL)->second );
		this->Ranging.RespondingDelay = std::stoi( ucnf.find(CFG::FIRMWARE::RESP_DELAY)->second );
		this->Ranging.FinalDelay =	std::stoi( ucnf.find(CFG::FIRMWARE::FINAL_DELAY)->second );

		this->state = ConfigFW::STATE::AVAILABLE;
	} catch (std::exception &e) {
		;
	}
}



ConfigFW::STATE ConfigFW::GetState() const
{
	return this->state;
}
