
#include "main.h"



int main()
{
	UserInterface ui(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::SIMPLEX_RD);
	PrintableUserPack upack;

	while (1) {
		if (ui.Read( upack ) == UserInterface::RESULT::SUCCESS) {
			upack.Print( cout );
			cout << endl;
		}
	}

	return 0;
}

