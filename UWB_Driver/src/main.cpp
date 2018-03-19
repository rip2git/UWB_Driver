
#include "COMHandler.h"
#include "main.h"
#include <thread>
#include <mutex>

/// linker, misc
/// -static-libgcc -static-libstdc++


#define POLL_PERIOD 	500


std::mutex mu;
unsigned char buf[128];
unsigned char buf_size;
unsigned int distance;
COMHandler hPort;
COMHandler::RESULT opRes;


void receive(void) {
	UserPack upack;
	cout << "***rd on" << endl;

	while (1) {
		mu.try_lock();
		opRes = hPort.Receive(&upack);
		mu.unlock();
		if (opRes == COMHandler::RESULT::SUCCESS) {
			if (upack.Command == UserPack::COMMAND::Status) {
				cout << "ERROR: " << static_cast <int> (upack.DestinationID) << endl;
			} else {
				upack.Print();
				cout << " distance: " << static_cast <int> (upack.Data[6] + (upack.Data[7] << 8)) << " cm" << endl;
			}
		}
	}
}


int main()
{
	std::string n;
	unsigned char poll_msg[] = "__OK__00";
	UserPack upack;

	int mainstate = 0;
	TON t1;

	if (hPort.GetState() == COMHandler::STATE::OPENED)
		cout << "*** COM" << static_cast <int> (hPort.GetPortNumber()) << " opened" << endl;
	else
		cout << "***error" << endl;


	t1.start(0);
	while (1) {
		switch (mainstate) {
			case 0:
			{
				upack.Command = UserPack::COMMAND::SetID;
				upack.DestinationID = hPort.GetPortNumber() & 0x0F;
				upack.TotalSize = 0;
				mu.try_lock();
				hPort.Send(&upack);
				opRes = hPort.Receive(&upack);
				mu.unlock();

				if (opRes == COMHandler::RESULT::SUCCESS) {
					for (uint8 i = 0; i < upack.TotalSize; ++i)
						cout << upack.Data[i];
					cout << endl;
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
				cout << "***wr on" << endl;
				mainstate = 2;
			} break;
			case 2:
			{
				//std::getline(cin, n);
				CrossSleep(500);
				mu.try_lock();
				hPort.Send(&upack);
				cout << "***sended: " << t1.since() << endl;
				mu.unlock();
			} break;

			default: // nop
				break;
		}
	}

	return 0;
}

