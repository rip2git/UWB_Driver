#include "main.h"


/// linker, misc
/// -static-libgcc -static-libstdc++

#define POLL_PERIOD 	500


Logger LOG;
std::mutex mu;
unsigned char buf[128];
unsigned char buf_size;
unsigned int distance;
COMHandler hPort;
COMHandler::RESULT opRes;


void receive(void) {
	UserPackFW upack;
	LOG << "***rd on" << endl;

	while (1) {
		mu.try_lock();
		opRes = hPort.Receive(&upack);
		mu.unlock();
		if (opRes == COMHandler::RESULT::SUCCESS) {
			switch (upack.Command) {
				case UserPack::COMMAND::Status:
				{
					LOG << "ERROR: " << static_cast <int> (upack.DestinationID) << endl;
				} break;

				default:
				{
					upack.Print( LOG );
					LOG << " distance: " << static_cast <int> (upack.Data[6] + (upack.Data[7] << 8)) << " cm" << endl;
				} break;
			}
		}
	}
}


int main()
{
	int mainstate = 0;
	TON t1;
	UserPackFW upack;
	uint8_t deviceID;
	Logger::MODE LM;

	string tmps = "__OK__00";
	vector <uint8_t> poll_msg;
	poll_msg.assign(tmps.begin(), tmps.end());
	tmps.clear();

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
		if ( ucnf.find(CFG::MAIN::DEVICE_ID) 	== ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::MAIN::LOG_MODE) 	== ucnf.end() ) throw std::exception();

		deviceID = static_cast <uint8_t> ( stoi(ucnf.find(CFG::MAIN::DEVICE_ID)->second) );
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


	t1.start(0);
	while (1) {
		switch (mainstate) {
			case 0:
			{
				upack.Command = UserPack::COMMAND::SetID;
				upack.DestinationID = deviceID;
				upack.TotalSize = 0;
				mu.try_lock();
				hPort.Send(&upack);
				opRes = hPort.Receive(&upack);
				mu.unlock();

				if (opRes == COMHandler::RESULT::SUCCESS) {
					for (uint8_t i = 0; i < upack.TotalSize; ++i)
						LOG << upack.Data[i];
					LOG << endl;
					mainstate = 1;
				}
			} break;
			case 1:
			{
				upack.Command = UserPack::COMMAND::Distance;
				upack.DestinationID = UserPack::BROADCAST_ID;
				upack.TotalSize = sizeof(poll_msg);
				upack.SetData(poll_msg);

				std::thread thr(receive);
				thr.detach();

				CrossSleep(11);
				LOG << "***wr on" << endl;
				mainstate = 2;
			} break;
			case 2:
			{
				CrossSleep(500);
				mu.try_lock();
				hPort.Send(&upack);
				LOG << "***sended: " << t1.since() << endl;
				mu.unlock();
			} break;

			default: // nop
				break;
		}
	}

	return 0;
}

