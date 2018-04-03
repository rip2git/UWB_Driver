
#include "main.h"


PrintableUserPack up1, up2;
void receive(void) {

	while (1) {
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;

		if (up1.DestinationID != 0)
			up1.Print( cout );
		else
			cout << endl;

		if (up2.DestinationID != 0)
			up2.Print( cout );

		CrossSleep(20);
	}
}



int main()
{
	UserInterface ui(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::SIMPLEX_RD);
	PrintableUserPack upack;

	up1.DestinationID = 0;
	up2.DestinationID = 0;

	std::thread thr(receive);
	thr.detach();
	CrossSleep(10);

	upack.Data.resize(8);
	bool fl = true;
	uint8_t prev_id = 0;
	if ( ui.Read(upack) == UserInterface::RESULT::SUCCESS) { // started
		while (1) {
			if ( ui.Read(upack) == UserInterface::RESULT::SUCCESS) {

				if (prev_id != upack.DestinationID && fl) {
					if (prev_id != 0) {
						up2 = upack;
						fl = false;
					} else {
						up1 = upack;
					}
					prev_id = upack.DestinationID;
				}

				if (up1.DestinationID == upack.DestinationID)
					up1 = upack;
				if (up2.DestinationID == upack.DestinationID)
					up2 = upack;

			}
		}
	}

	return 0;
}


