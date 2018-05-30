//#include "main.h"




/*TON t1;
Logger LOG;
std::mutex mu;
COMHandler hPort;
COMHandler::RESULT opRes;
UserInterface *ui;
UserInterface::RESULT uiRes;
UserInterfaceDBG *uiDBG;



void polling();
void init_debug_ui(void);
void receive(void);
bool send_data(const DataConfig &dataConfig, const std::vector<UserPackFW> &upackFW);
UserPackHL recv_data(const DataConfig &dataConfig);*/


/// linker, misc
/// -static-libgcc -static-libstdc++
#include "Driver.h"


int main()
{
	Driver drv;
	drv.Initialization();
	drv.StartProcess();
	return 0;
}

	/*// DRIVER CONFIGURATION *******************************************************************
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

		} catch (std::exception &e) {
			std::cerr << "config error -> exit" << std::endl;
			exit(0);
		}

		// switch logger --------------------------------------------------------------------------
		switch (LM) {
		case Logger::MODE::OFF:
			LOG.toNull();
			break;
		case Logger::MODE::CONSOLE:
			LOG.toConsole();
			break;
		case Logger::MODE::FILE:
			LOG.toFile();
			break;
		}

		// opens COM port -------------------------------------------------------------------------
		if (hPort.GetState() == COMHandler::STATE::OPENED) {
			LOG << "*** COM" << static_cast <int> (hPort.GetPortNumber()) << " opened" << endl;
		} else {
			LOG << "*** COM error -> exit" << endl;
			exit(0);
		}

		ui = new UserInterface(UserInterface::MODE::CREATE_NEW, UserInterface::CONNTYPE::DUPLEX);
		ui->Initialization();

		std::thread thr(init_debug_ui);
		thr.detach();
	}
	// END OF DRIVER CONFIGURATION ************************************************************

	int mainstate = 0;
	UserPackFW upack;

	t1.start(0);
	while (1) {
		switch (mainstate) {
			case 0: // config init
			{
				ConfigFW configFW;
				configFW.ToUserPackFW(upack);

				if (configFW.GetState() != ConfigFW::STATE::AVAILABLE) {
					LOG << "*** CONFIG FIRMWARE error -> exit" << endl;
					exit(0);
				}
				mainstate = 1;
			} break;
			case 1: // config send while fails
			{
				hPort.Send(upack);
				CrossSleep(10);
				if (hPort.Receive(upack) == COMHandler::RESULT::SUCCESS) {
					upack.Print( LOG );
					mainstate = 2;
				} else {
					mainstate = 0; // restart
				}
			} break;
			case 2: // receiving thread create
			{
				std::thread thr(receive);
				thr.detach();

				CrossSleep(10);
				mainstate = 3;
			} break;
			case 3: // polling
			{
				LOG << "***wr on" << endl;
				polling();
				mainstate = 0; // restart
			} break;

			default: // nop
				break;
		}
	}

	return 0;
}



void polling()
{
	UserPackHL upackHL;
	std::vector<UserPackFW> upackFW;
	DataConfig dataConfig;
	dataConfig.ReadConfig();



	while (1) {
		if (ui->Read(upackHL) == UserInterface::RESULT::SUCCESS) {
			upackFW = UserPacksConverter::ToFW(upackHL, dataConfig.BufferSize);

			mu.try_lock();
			if (upackFW.size() == 1) {
				hPort.Send(upackFW[0]);
				LOG << "***sended: " << t1.since() <<  " --- ";
				upackFW[0].Print( LOG );
			} else if (upackHL.FCmd == UserPackHL::FCommand::Data &&
				dataConfig.GetState() == DataConfig::STATE::AVAILABLE
			) {
				dataConfig.Destination = upackHL.SCmd;
				dataConfig.Parts = upackFW.size();
				dataConfig.Transactions = dataConfig.Parts / dataConfig.TransactionSize;
				dataConfig.Transactions += (dataConfig.Parts % dataConfig.TransactionSize == 0)?
						0 : 1;

				std::vector <UserPackFW> upackFW_part;
				UserPackFW upack_service;

				int it = 0;
				for (dataConfig.TransactionNum = 1; dataConfig.TransactionNum <= dataConfig.Transactions; ++dataConfig.TransactionNum) {
					upackFW_part.assign(upackFW.begin() + it, upackFW.begin() + it + dataConfig.TransactionSize);
					it += dataConfig.TransactionSize;
					if ( !send_data(dataConfig, upackFW) )
						break;
				}
			}
			mu.unlock();
		}
	}
}



bool send_data(const DataConfig &dataConfig, const std::vector <UserPackFW> &upackFW)
{
	int i = 0;
	UserPackFW upack_service;

	dataConfig.ToUserPackFW(upack_service);
	hPort.Send(upack_service);

	if (hPort.Receive(upack_service) == COMHandler::RESULT::SUCCESS &&
		upack_service.FCmd == UserPackFW::FCommand::Service &&
		upack_service.SCmd == UserPackFW::FCommand::Ack
	) {
		for (; i < 2 && i < upackFW.size(); ++i) {
			hPort.Send(upackFW[i]);
			LOG << "***sended: " << t1.since() <<  " --- ";
			upackFW[i].Print( LOG );
		}
		for (; i < upackFW.size(); ++i) {
			if (hPort.Receive(upack_service) == COMHandler::RESULT::SUCCESS &&
				upack_service.FCmd == UserPackFW::FCommand::Service &&
				upack_service.SCmd == UserPackFW::FCommand::Ack
			) {
				hPort.Send(upackFW[i]);
				LOG << "***sended: " << t1.since() <<  " --- ";
				upackFW[i].Print( LOG );
			} else {
				return false;
			}
		}
		// todo последнее подтверждение уходит к receive
		return true;
	}
	return false;
}



UserPackHL recv_data(const DataConfig &dataConfig)
{
	std::vector<UserPackFW> upackFW(dataConfig.Parts);



	return UserPacksConverter::ToHL(upackFW);
}



void receive(void)
{
	UserPackFW upackFW;
	UserPackHL upackHL;
	LOG << "***rd on" << endl;
	while (1) {
		mu.try_lock();
		opRes = hPort.Receive(upackFW);
		mu.unlock();
		if (opRes == COMHandler::RESULT::SUCCESS) {
			upackHL = UserPacksConverter::ToHL(upackFW);
			switch (upackFW.FCmd) {
				case UserPack::FCommand::Service: {
					if (uiDBG->GetState() == UserInterfaceDBG::STATE::OPENED) {
						uiDBG->Write(upackHL);
					}
				} break;
				case UserPack::FCommand::DataConfig: {
					DataConfig dataConfig;
					dataConfig.ToStruct(upackFW);
					LOG << "***recved: " << t1.since() <<  " --- ";
					upackFW.Print( LOG );
					if (dataConfig.GetState() == DataConfig::STATE::AVAILABLE) {
						upackHL = recv_data(dataConfig);
						ui->Write(upackHL);
					}
				} break;
				default:
				{
					LOG << "***recved: " << t1.since() <<  " --- ";
					upackFW.Print( LOG );
					ui->Write(upackHL);
				} break;
			}
		}
	}
}



void init_debug_ui(void)
{
	uiDBG = new UserInterfaceDBG(UserInterfaceDBG::MODE::CREATE_NEW, UserInterfaceDBG::CONNTYPE::SIMPLEX_WR);
	uiDBG->Initialization();
	return;
}*/

