
#include "Driver.h"


using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::map;



void Driver::Receive(void)
{
	UserPackFW upackFW;
	UserPackHL upackHL;
	COMHandler::RESULT opRes;

	*this->LOG << "***rd on" << endl;
	while (1) {
		this->uiMutex->try_lock();
		opRes = this->hPort->Receive(upackFW);
		this->uiMutex->unlock();

		if (opRes == COMHandler::RESULT::SUCCESS) {
			upackHL = UserPacksConverter::ToHL(upackFW);

			switch (upackFW.FCmd) {
				case UserPack::FCommand::Service: {
					if (this->uiDBG->GetState() == UserInterfaceDBG::STATE::OPENED) {
						this->uiDBG->Write(upackHL);
					}
				} break;

				// todo
//				case UserPack::FCommand::DataConfig: {
//					DataConfig dataConfig;
//					dataConfig.ToStruct(upackFW);
//					*this->LOG << "***recved: " << this->t1->since() <<  " --- ";
//					upackFW.Print( *this->LOG );
//					if (dataConfig.GetState() == DataConfig::STATE::AVAILABLE) {
//						upackHL = this->ReceiveData(dataConfig);
//						this->ui->Write(upackHL);
//					}
//				} break;

				default:
				{
					*this->LOG << "***recved: " << this->t1->since() <<  " --- ";
					upackFW.Print( *this->LOG );
					this->ui->Write(upackHL);
				} break;
			}
		}

	}
}



bool Driver::SendData(const DataConfig &dataConfig, const std::vector<UserPackFW> &upackFW)
{
	// todo
//	int i = 0;
//	UserPackFW upack_service;
//
//	dataConfig.ToUserPackFW(upack_service);
//	this->hPort->Send(upack_service);
//
//	if (this->hPort->Receive(upack_service) == COMHandler::RESULT::SUCCESS &&
//		upack_service.FCmd == UserPackFW::FCommand::Service &&
//		upack_service.SCmd == UserPackFW::FCommand::Ack
//	) {
//		for (; i < 2 && i < upackFW.size(); ++i) {
//			this->hPort->Send(upackFW[i]);
//			*this->LOG << "***sended: " << t1->since() <<  " --- ";
//			upackFW[i].Print( *this->LOG );
//		}
//		for (; i < upackFW.size(); ++i) {
//			if (this->hPort->Receive(upack_service) == COMHandler::RESULT::SUCCESS &&
//				upack_service.FCmd == UserPackFW::FCommand::Service &&
//				upack_service.SCmd == UserPackFW::FCommand::Ack
//			) {
//				this->hPort->Send(upackFW[i]);
//				*this->LOG << "***sended: " << this->t1->since() <<  " --- ";
//				upackFW[i].Print( *this->LOG );
//			} else {
//				return false;
//			}
//		}
//		// todo последнее подтверждение уходит к Receive
//		return true;
//	}
	return false;
}



UserPackHL Driver::ReceiveData(const DataConfig &dataConfig)
{
	// todo
	std::vector<UserPackFW> upackFW(dataConfig.Parts);


	return UserPacksConverter::ToHL(upackFW);
}



void Driver::Polling()
{
	UserPackHL upackHL;
	std::vector<UserPackFW> upackFW;
	DataConfig dataConfig;
	dataConfig.ReadConfig();

	while (1) {
		if (this->ui->Read(upackHL) == UserInterface::RESULT::SUCCESS) {
			upackFW = UserPacksConverter::ToFW(upackHL, dataConfig.BufferSize);

			if (upackFW.size() == 1) {
				this->uiMutex->try_lock();
				this->hPort->Send(upackFW[0]);
				*this->LOG << "***sended: " << this->t1->since() <<  " --- ";
				upackFW[0].Print( *this->LOG );
				this->uiMutex->unlock();
			}
//			else if (upackHL.FCmd == UserPackHL::FCommand::Data &&
//				dataConfig.GetState() == DataConfig::STATE::AVAILABLE
//			) {
//				dataConfig.Destination = upackHL.SCmd;
//				dataConfig.Parts = upackFW.size();
//				dataConfig.Transactions = dataConfig.Parts / dataConfig.TransactionSize;
//				dataConfig.Transactions += (dataConfig.Parts % dataConfig.TransactionSize == 0)?
//						0 : 1;
//
//				std::vector <UserPackFW> upackFW_part;
//				UserPackFW upack_service;
//
//				int it = 0;
//				for (dataConfig.TransactionNum = 1; dataConfig.TransactionNum <= dataConfig.Transactions; ++dataConfig.TransactionNum) {
//					// todo
//					this->uiMutex->try_lock();
//					upackFW_part.assign(upackFW.begin() + it, upackFW.begin() + it + dataConfig.TransactionSize);
//					it += dataConfig.TransactionSize;
//					if ( !this->SendData(dataConfig, upackFW) )
//						break;
//					this->uiMutex->unlock();
//				}
//			}

		}
	}
}



void Driver::StartProcess()
{
	int mainstate = 0;
	UserPackFW upack;

	*this->LOG << "*** Started" << endl;

	this->t1->start(0);
	while (1) {
		switch (mainstate) {
			case 0: // config init
			{
				ConfigFW configFW;
				configFW.ToUserPackFW(upack);

				if (configFW.GetState() != ConfigFW::STATE::AVAILABLE) {
					*this->LOG << "*** CONFIG FIRMWARE error -> exit" << endl;
					exit(0);
				}
				mainstate = 1;
			} break;
			case 1: // config send while fails
			{
				this->hPort->Send(upack);
				CrossSleep(10);
				if (this->hPort->Receive(upack) == COMHandler::RESULT::SUCCESS) {
					upack.Print( *this->LOG );
					mainstate = 2;
				} else {
					mainstate = 0; // restart
				}
			} break;
			case 2: // receiving thread create
			{
				this->receivingThread = std::thread(&Driver::Receive, this);
				this->receivingThread.detach();
				CrossSleep(10);

				mainstate = 3;
			} break;
			case 3: // polling
			{
				*this->LOG << "***wr on" << endl;
				this->Polling();
				mainstate = 0; // restart
			} break;

			default: // nop
				break;
		}
	}
}



void Driver::Initialization(void)
{
	this->t1 = new TON();
	this->LOG = new Logger();
	this->uiMutex = new std::mutex();
	this->wrMutex = new std::mutex();

	Logger::MODE LM;
	// reads config ---------------------------------------------------------------------------
	try {
		std::map <std::string, std::string> ucnf;
		IniFile ini(CFG::FILE_NAME);

		if (ini.isOpened())
			ini.Parse();
		else
			throw std::exception();

		ucnf = ini.GetSection(CFG::MAIN::SECTION);

		// prevents segmentation fault from stoi
		if ( ucnf.find(CFG::MAIN::LOG_MODE) == ucnf.end() ) throw std::exception();

		LM = static_cast <Logger::MODE> ( stoi(ucnf.find(CFG::MAIN::LOG_MODE)->second) );

	} catch (std::exception &e) {
		std::cerr << "config error -> exit" << std::endl;
		exit(0);
	}

	// switch logger --------------------------------------------------------------------------
	switch (LM) {
	case Logger::MODE::OFF:
		this->LOG->toNull();
		break;
	case Logger::MODE::CONSOLE:
		this->LOG->toConsole();
		break;
	case Logger::MODE::FILE:
		this->LOG->toFile();
		break;
	}

	// opens COM port -------------------------------------------------------------------------
	this->hPort = new COMHandler();
	if (this->hPort->GetState() == COMHandler::STATE::OPENED) {
		*this->LOG << "*** Port" << static_cast <int> (this->hPort->GetPortNumber()) << " opened" << endl;
	} else {
		*this->LOG << "*** Port error -> exit" << endl;
		exit(0);
	}

	this->ui = new UserInterface(UserInterface::MODE::CREATE_NEW, UserInterface::CONNTYPE::DUPLEX);
	this->uiDBG = new UserInterfaceDBG(UserInterfaceDBG::MODE::CREATE_NEW, UserInterfaceDBG::CONNTYPE::SIMPLEX_WR);
	this->ui->Initialization();

	this->initDBGThread = std::thread(&Driver::InitDebugUI, this);
	this->initDBGThread.detach();
	CrossSleep(10);
}



void Driver::InitDebugUI(void)
{
	this->uiDBG->Initialization();
	return;
}



Driver::~Driver()
{
	delete this->t1;
	delete this->LOG;
	delete this->uiMutex;
	delete this->wrMutex;
	delete this->hPort;
	delete this->ui;
	delete this->uiDBG;
}
