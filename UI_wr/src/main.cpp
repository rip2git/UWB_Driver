#include "main.h"

/// linker, misc
/// -static-libgcc -static-libstdc++



uint8_t buf[256];



void receive(void) {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
	while (1) {
		cout << "Данные: ";
		cin >> buf;
	}
}



int main()
{
	UserInterface ui(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::SIMPLEX_WR);
	UserPackHL upack;

	std::thread thr(receive);
	thr.detach();
	CrossSleep(10);

	buf[0] = 'a';
	buf[1] = 'b';
	buf[2] = 'c';
	buf[3] = 'd';
	buf[4] = 'e';
	buf[5] = 'f';

	upack.Command = UserPackHL::COMMAND::Distance;
	upack.DestinationID = UserPackHL::BROADCAST_ID;
	upack.TotalSize = 6;
	while (1) {
		upack.Data.assign(&(buf[0]), &(buf[6]));
		ui.Write(upack);
		CrossSleep(500);
	}

	return 0;
}
