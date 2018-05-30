
#include "DataConfig.h"
#include "CFG.h"
#include "IniFiles.h"



void DataConfig::ToStruct(const UserPackFW &pack)
{
	if (pack.TotalSize == this->configSize) {
		uint8_t i = 0;
		this->Destination = pack.Data[i++];
		this->Parts = pack.Data[i++];
		this->Parts |= pack.Data[i++] << 8;
		this->BufferSize = pack.Data[i++];
		this->TransactionSize = pack.Data[i++];
		this->Transactions = pack.Data[i++];
		this->Transactions |= pack.Data[i++] << 8;
		this->TransactionNum = pack.Data[i++];
		this->TransactionNum |= pack.Data[i++] << 8;
		this->TrustPacks = pack.Data[i++];
		this->Repeats = pack.Data[i++];

		this->state = DataConfig::STATE::AVAILABLE;
	} else {
		this->state = DataConfig::STATE::ERROR;
	}
}



void DataConfig::ToUserPackFW(UserPackFW &pack) const
{
	if (this->configSize  <= UserPackFW::MAX_DATA_SIZE) {
		uint8_t i = 0;
		pack.FCmd = UserPackFW::FCommand::DataConfig;
		pack.SCmd = 0;
		pack.TotalSize = this->configSize;
		pack.Data.resize(pack.TotalSize);
		//
		pack.Data[i++] = this->Destination;
		pack.Data[i++] = this->Parts;
		pack.Data[i++] = this->Parts >> 8;
		pack.Data[i++] = this->BufferSize;
		pack.Data[i++] = this->TransactionSize;
		pack.Data[i++] = this->Transactions;
		pack.Data[i++] = this->Transactions >> 8;
		pack.Data[i++] = this->TransactionNum;
		pack.Data[i++] = this->TransactionNum >> 8;
		pack.Data[i++] = this->TrustPacks;
		pack.Data[i++] = this->Repeats;
		this->state = DataConfig::STATE::AVAILABLE;
	} else {
		this->state = DataConfig::STATE::ERROR;
	}
}



DataConfig::DataConfig()
{
	this->state = DataConfig::STATE::ERROR;
}



void DataConfig::ReadConfig()
{
	try {
		std::map <std::string, std::string> ucnf;
		IniFile ini(CFG::FILE_NAME);

		if (ini.isOpened())
			ini.Parse();
		else
			throw std::exception();

		ucnf = ini.GetSection(CFG::DATA::SECTION);

		// prevents segmentation fault from stoi
		if ( ucnf.find(CFG::DATA::BUFFER_SIZE) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::DATA::TRANSACTION_SIZE) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::DATA::TRUST_PACKS) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::DATA::REPEATS) == ucnf.end() ) throw std::exception();

		this->BufferSize = std::stoi( ucnf.find(CFG::DATA::BUFFER_SIZE)->second );
		this->TransactionSize = std::stoi( ucnf.find(CFG::DATA::TRANSACTION_SIZE)->second );
		this->TrustPacks = std::stoi( ucnf.find(CFG::DATA::TRUST_PACKS)->second );
		this->Repeats =	std::stoi( ucnf.find(CFG::DATA::REPEATS)->second );
		//
		this->Destination = 0;
		this->Parts = 0;
		this->Transactions = 0;
		this->TransactionNum = 0;

		this->state = DataConfig::STATE::AVAILABLE;
	} catch (std::exception &e) {
		this->state = DataConfig::STATE::ERROR;
	}
}



DataConfig::STATE DataConfig::GetState() const
{
	return this->state;
}
