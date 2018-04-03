#include "main.h"

/// linker, misc
/// -static-libgcc -static-libstdc++


Logger LOG;
std::mutex mu;
COMHandler hPort;
COMHandler::RESULT opRes;
UserInterface *ui;
UserInterface::RESULT uiRes;



void receive(void) {
	UserPackFW upackFW;
	UserPackHL upackHL;
	LOG << "***rd on" << endl;
	while (1) {
		mu.try_lock();
		opRes = hPort.Receive(upackFW);
		mu.unlock();
		if (opRes == COMHandler::RESULT::SUCCESS) {
			switch (upackFW.Command) {
				case UserPack::COMMAND::Status:
				{
					LOG << "ERROR: " << static_cast <int> (upackFW.DestinationID) << endl;
				} break;

				default:
				{
					upackHL = UserPacksConverter::ToHL(upackFW);
					ui->Write(upackHL);
					//
					upackFW.Print( LOG );
					LOG << " distance: " << static_cast <int> (upackFW.Data[6] + (upackFW.Data[7] << 8)) << " cm" << endl;
				} break;
			}
		}
	}
}


int main()
{
	// DRIVER CONFIGURATION *******************************************************************
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
	}
	// END OF DRIVER CONFIGURATION ************************************************************

	int mainstate = 0;
	TON t1;
	UserPackHL upackHL;
	std::vector<UserPackFW> upackFW(1);
	t1.start(0);
	while (1) {
		switch (mainstate) {
			case 0: // config init
			{
				ConfigFW configFW;
				configFW.ToUserPackFW(upackFW[0]);

				if (configFW.GetState() != ConfigFW::STATE::AVAILABLE) {
					LOG << "*** CONFIG FIRMWARE error -> exit" << endl;
					exit(0);
				}
				mainstate = 1;
			} break;
			case 1: // config send while fails
			{
				hPort.Send(upackFW[0]);
				if (hPort.Receive(upackFW[0]) == COMHandler::RESULT::SUCCESS) {
					for (uint8_t i = 0; i < upackFW[0].TotalSize; ++i)
						LOG << upackFW[0].Data[i];
					LOG << endl;

					upackHL = UserPacksConverter::ToHL(upackFW[0]);
					mainstate = 2;
				}
			} break;
			case 2: // answer send while fail
			{
				if (ui->Write(upackHL) == UserInterface::RESULT::SUCCESS)
					mainstate = 3;
			} break;
			case 3: // receiving thread create
			{
				std::thread thr(receive);
				thr.detach();

				CrossSleep(10);
				LOG << "***wr on" << endl;

				mainstate = 4;
			} break;
			case 4: // polling
			{
				if (ui->Read(upackHL) == UserInterface::RESULT::SUCCESS) {
					upackFW = UserPacksConverter::ToFW(upackHL);

					// TODO >1
					if (upackFW.size() == 1) {
						mu.try_lock();
						hPort.Send(upackFW[0]);
						LOG << "***sended: " << t1.since() << endl;
						upackFW[0].Print( LOG );
						LOG << endl;
						mu.unlock();
					}
				}
			} break;

			default: // nop
				break;
		}
	}

	return 0;
}

