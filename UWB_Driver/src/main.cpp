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


#ifdef MAIN_DEBUG
/* TODO: testing space. Include testing code here */
	uint8_t c;
	bool fl = false;
	int res;
	TON t1;
	t1.start(0);
	while (1) {
		mu.try_lock();
		res = hPort.GetPortDirectly().ReadByte(&c);
		mu.unlock();
		if (res == 1) {
			if (fl) {
				fl = false;
				LOG << t1.since() / 1000 << "s " << t1.since() % 1000 << "ms ---> ";
			}
			LOG << static_cast<char>(c);
			if (c == '\n')
				fl = true;
		}
	}
/* TODO: end of testing code*/
#else
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
#endif
}


int main()
{
	char c = '0';
	int mainstate = 0;
	TON t1;
	UserPackFW upack;
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
		if ( ucnf.find(CFG::MAIN::LOG_MODE) 	== ucnf.end() ) throw std::exception();
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
				ConfigFW configFW;
				configFW.ToUserPackFW(upack);

#ifdef MAIN_DEBUG
/* TODO: testing space. Include testing code here */
				hPort.Send(&upack);
				mainstate = 1;
/* TODO: end of testing code*/
#else
				hPort.Send(&upack);
				opRes = hPort.Receive(&upack);
				if (opRes == COMHandler::RESULT::SUCCESS) {
					for (uint8_t i = 0; i < upack.TotalSize; ++i)
						LOG << upack.Data[i];
					LOG << endl;
					mainstate = 1;
				}
#endif
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
#ifdef MAIN_DEBUG
/* TODO: testing space. Include testing code here */
				cout << "***wr on" << endl;
/* TODO: end of testing code*/
#else
				LOG << "***wr on" << endl;
#endif
				mainstate = 2;
			} break;
			case 2:
			{
				CrossSleep(500);
				mu.try_lock();
				//
				upack.Data[0] = c;
				c++;
				if (c > '9') c = '0';
				//
				hPort.Send(&upack);
#ifdef MAIN_DEBUG
/* TODO: testing space. Include testing code here */
				cout << "***sended: " << t1.since() << endl;
/* TODO: end of testing code*/
#else
				LOG << "***sended: " << t1.since() << endl;
#endif
				mu.unlock();
			} break;

			default: // nop
				break;
		}
	}

	return 0;
}

