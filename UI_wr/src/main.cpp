
#include "main.h"



UserInterface ui(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::SIMPLEX_WR);
UserPackHL upack;
std::mutex mu;
bool send_data_fl = false;



void send_data(void) {
	while (1) {
		if (send_data_fl) {
			mu.try_lock();
			ui.Write(upack);
			mu.unlock();
			send_data_fl = false;
		}
		CrossSleep(20);
	}
}




void send_dist(void) {
	UserPackHL dupack;

	dupack.FCmd = UserPackHL::FCommand::Distance;
	dupack.SCmd = UserPackHL::BROADCAST_ID;
	dupack.TotalSize = 6;
	dupack.Data.resize(dupack.TotalSize);
	uint8_t val = 'x';
	std::fill(dupack.Data.begin(), dupack.Data.end(), val);

	while (1) {
		mu.try_lock();
		ui.Write(dupack);
		CrossSleep(50);
		mu.unlock();
		CrossSleep(450);
	}
}




int main()
{
	std::thread thr1(send_dist);
	std::thread thr2(send_data);
	thr1.detach();
	thr2.detach();
	CrossSleep(10);

	cout << "Enter data:\nExample:\"id-123456\" ---> \"1-123456\"" << endl;

	uint8_t buffer[256];

	int i;
	while (1) {
		cin >> buffer;

		i = 0;
		while (buffer[i++] != '\0');
		upack.FCmd = UserPackHL::FCommand::Data;
		upack.SCmd = buffer[0] & 0x0F;
		upack.TotalSize = i-3;
		upack.Data.assign(&(buffer[2]), &(buffer[i-1]));
		send_data_fl = true;
	}

	return 0;
}
