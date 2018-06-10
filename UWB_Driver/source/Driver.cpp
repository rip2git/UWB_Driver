
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
	bool toUser = false;

	*this->LOG << *this->t1 << "Read-thread started" << endl;
	while (1) {
		opRes = this->hPort->Receive(upackFW);

		if (opRes == COMHandler::RESULT::SUCCESS) {
			*this->LOG << *this->t1 << "FW: ";
			upackFW.Print( *this->LOG );

			upackHL = UserPacksConverter::ToHL(upackFW);

			if ( UserPackFW::isCommand(upackFW.FCmd) ) {
				switch (upackFW.FCmd.Cmd)
				{
				// ------------------------------------------------------------------------
				case UserPack::Command::Service: {
					if (this->uiDBG->GetState() == UserInterfaceDBG::STATE::OPENED) {
						this->uiDBG->Write(upackHL);
					}
				} break;
				// ------------------------------------------------------------------------
				case UserPack::Command::SystemConfig: {
					this->signals.reConfig = Signal::Set;
					return;
				} break;
				// ------------------------------------------------------------------------
				default: {
					toUser = true;
				} break;
				// ------------------------------------------------------------------------
				}
			} else if ( UserPackFW::isResult(upackFW.FCmd) ) {
				switch (upackFW.FCmd.Res) {
				// ------------------------------------------------------------------------
				case UserPackFW::CommandResult::Accepted: {
					this->signals.accepted = Signal::Set;
				} break;
				// ------------------------------------------------------------------------
				case UserPackFW::CommandResult::Success: {
					this->signals.success = Signal::Set;
				} break;
				// ------------------------------------------------------------------------
				case UserPackFW::CommandResult::Fail: {
					this->signals.fail = Signal::Set;
				} break;
				// ------------------------------------------------------------------------
				}
			}

			if (toUser) {
				toUser = false;
				this->wrMutex->lock();
				this->ui->Write(upackHL);
				this->wrMutex->unlock();
			}
		}
	}
}
// todo
//				case UserPack::Command::DataConfig: {
//					DataConfig dataConfig;
//					dataConfig.ToStruct(upackFW);
//					*this->LOG << *this->t1 << "FW: ";
//					upackFW.Print( *this->LOG );
//					if (dataConfig.GetState() == DataConfig::STATE::AVAILABLE) {
//						upackHL = this->ReceiveData(dataConfig);
//						this->ui->Write(upackHL);
//					}
//				} break;



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
//			*this->LOG << *this->t1 << "DRV: ";
//			upackFW[i].Print( *this->LOG );
//		}
//		for (; i < upackFW.size(); ++i) {
//			if (this->hPort->Receive(upack_service) == COMHandler::RESULT::SUCCESS &&
//				upack_service.FCmd == UserPackFW::FCommand::Service &&
//				upack_service.SCmd == UserPackFW::FCommand::Ack
//			) {
//				this->hPort->Send(upackFW[i]);
//				*this->LOG << *this->t1 << "DRV: ";
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
	std::vector <UserPackFW> upackFW;
	UserPackFW retupackFW;
	DataConfig dataConfig;
	dataConfig.ReadConfig();

	TON pollingTimer;
	int repeats;
	while (1) {
		if (this->signals.reConfig == Signal::Set)
			return;

		if (this->ui->Read(upackHL) == UserInterface::RESULT::SUCCESS) {
			upackFW = UserPacksConverter::ToFW(upackHL, dataConfig.BufferSize);

			this->signals.success = Signal::Reset;
			this->signals.fail = Signal::Reset;
			this->signals.accepted = Signal::Reset;

			if (upackFW.size() == 1) {
				pollingTimer.start(this->pollingPeriod);
				do { // waiting for accept-result
					this->hPort->Send(upackFW[0]);
					*this->LOG << *this->t1 << "DRV: ";
					upackFW[0].Print( *this->LOG );
					CrossSleep(this->pollingPeriod / 10);
					if ( this->signals.accepted == Signal::Set )
						break;
				} while ( !pollingTimer.check() );

				// waiting for timeout || fail || success of operation
				while ( !pollingTimer.check() &&
						this->signals.fail == Signal::Reset &&
						this->signals.success == Signal::Reset
				) {
					CrossSleep(this->pollingPeriod / 10);
				}

				upackHL.SCmd.Cmd = upackHL.FCmd.Cmd;
				upackHL.FCmd.Res = (this->signals.success == Signal::Set)?
						UserPackHL::CommandResult::Success : UserPackHL::CommandResult::Fail;
				upackHL.TotalSize = 0;

				this->wrMutex->lock();
				this->ui->Write(upackHL);
				this->wrMutex->unlock();

				pollingTimer.reset();
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
	COMHandler::RESULT CHRes;
	UserPackFW upack;
	int mainstate = 0;

	*this->LOG << *this->t1 << "High Level is connected -> Start" << endl;

	while (1) {
		switch (mainstate) {
			case 0: // config init
			{
				ConfigFW configFW;
				configFW.ToUserPackFW(upack);

				if (configFW.GetState() != ConfigFW::STATE::AVAILABLE) {
					*this->LOG << *this->t1 << "CONFIG FIRMWARE error -> Exit" << endl;
					exit(0);
				}
				mainstate = 1;
			} break;
			case 1: // config send while fails
			{
				this->hPort->Send(upack);
				CrossSleep(10);

				do {
					CHRes = this->hPort->Receive(upack);
					if (CHRes != COMHandler::RESULT::SUCCESS)
						break;
					*this->LOG << *this->t1 << "FW: ";
					upack.Print( *this->LOG );

					if (upack.FCmd == UserPack::CommandResult::Success &&
						upack.SCmd == UserPack::Command::SystemConfig
					) {
						this->signals.reConfig = Signal::Reset;
						mainstate = 2;
						break;
					} else {
						mainstate = 0; // restart
					}
				} while (CHRes == COMHandler::RESULT::SUCCESS);

			} break;
			case 2: // receiving thread create
			{
				this->receivingThread = new std::thread(&Driver::Receive, this);
				this->receivingThread->detach();
				CrossSleep(10);

				mainstate = 3;
			} break;
			case 3: // polling
			{
				*this->LOG << *this->t1 << "Write-thread started" << endl;
				this->Polling();

				delete this->receivingThread;
				this->receivingThread = nullptr;
				*this->LOG << *this->t1 << "Firmware requested the configuration -> End threads, Restart" << endl;
				mainstate = 0; // restart
			} break;

			default: // nop
				break;
		}
	}
}



void Driver::Initialization(void)
{
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

		ucnf = ini.GetSection(CFG::DATA::SECTION);
		if ( ucnf.find(CFG::DATA::REPEATS) == ucnf.end() ) throw std::exception();
		this->repeats = static_cast <int> ( stoi(ucnf.find(CFG::DATA::REPEATS)->second) );

		ucnf = ini.GetSection(CFG::FIRMWARE::SECTION);
		if ( ucnf.find(CFG::FIRMWARE::POLL_PERIOD) == ucnf.end() ) throw std::exception();
		this->pollingPeriod = static_cast <int> ( stoi(ucnf.find(CFG::FIRMWARE::POLL_PERIOD)->second) );

	} catch (std::exception &e) {
		std::cerr << "CONFIG error -> Exit" << std::endl;
		exit(0);
	}

	// switch logger --------------------------------------------------------------------------
	this->LOG->SwitchMode( LM );

	// opens COM port -------------------------------------------------------------------------
	this->hPort = new COMHandler();
	if (this->hPort->GetState() == COMHandler::STATE::OPENED) {
		*this->LOG << *this->t1 << "Port" << static_cast <int> (this->hPort->GetPortNumber()) << " is opened" << endl;
	} else {
		*this->LOG << *this->t1 << "Port error -> Exit" << endl;
		exit(0);
	}

	this->ui = new UserInterface(UserInterface::MODE::CREATE_NEW, UserInterface::CONNTYPE::DUPLEX);
	this->ui->Initialization();

	this->uiDBG = new UserInterfaceDBG(UserInterfaceDBG::MODE::CREATE_NEW, UserInterfaceDBG::CONNTYPE::SIMPLEX_WR);
	this->initDBGThread = new std::thread(&Driver::InitDebugUI, this);
	this->initDBGThread->detach();
	CrossSleep(10);
}



void Driver::InitDebugUI(void)
{
	this->uiDBG->Initialization();
	*this->LOG << *this->t1 << "Debugging is attached" << endl;
	return;
}



Driver::Driver()
{
	this->LOG = new Logger();
	this->initDBGThread = nullptr;
	this->receivingThread = nullptr;
	this->wrMutex = new std::mutex();
	this->hPort = nullptr;
	this->ui = nullptr;
	this->uiDBG = nullptr;
	this->repeats = 0;
	this->signals.reConfig = Signal::Reset;
	this->signals.accepted = Signal::Reset;
	this->signals.success = Signal::Reset;

	this->t1 = new TON();
	this->t1->start(0);
}



Driver::~Driver()
{
	delete this->t1;
	delete this->LOG;
	delete this->initDBGThread;
	delete this->receivingThread;
	delete this->wrMutex;
	delete this->hPort;
	delete this->ui;
	delete this->uiDBG;
}
