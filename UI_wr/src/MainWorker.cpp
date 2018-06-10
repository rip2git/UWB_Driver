
#include "MainWorker.h"

#include <thread>
#include <mutex>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;



void MainWorker::Start()
{
	this->UI_pipeData->Initialization();
	this->dataThread = std::thread(&MainWorker::sendData, this);
	this->dataThread.detach();
	this->distanceThread = std::thread(&MainWorker::sendDistance, this);
	this->distanceThread.detach();
	CrossSleep(10);

	cout << "Every 500 ms the writer sends distance-initiation command" << endl;
	cout << "Enter data:\nExample:\"id-123456\" ---> \"1-123456\"" << endl;

	uint8_t buffer[256];

	int i;
	while (1) {
		cin >> buffer;

		i = 0;
		while (buffer[i++] != '\0');
		this->UPackData.FCmd = UserPackHL::Command::Data;
		this->UPackData.SCmd = buffer[0] & 0x0F;
		this->UPackData.TotalSize = i-3;
		this->UPackData.Data.assign(&(buffer[2]), &(buffer[i-1]));
		this->send_data_fl = true;
	}
}



void MainWorker::sendDistance(void)
{
	UserPackHL dupack;
	dupack.FCmd = UserPackHL::Command::Distance;
	dupack.SCmd = UserPackHL::BROADCAST_ID;
	dupack.TotalSize = 6;
	dupack.Data.resize(dupack.TotalSize);
	uint8_t val = 'x';
	std::fill(dupack.Data.begin(), dupack.Data.end(), val);

	while (1) {
		this->mu.lock();
		this->UI_pipeData->Write(dupack);
		CrossSleep(50);
		this->mu.unlock();
		CrossSleep(450);
	}
}



void MainWorker::sendData(void)
{
	while (1) {
		if (this->send_data_fl) {
			this->mu.lock();
			this->UI_pipeData->Write(this->UPackData);
			this->mu.unlock();
			this->send_data_fl = false;
		}
		CrossSleep(20);
	}
}



MainWorker::MainWorker()
{
    this->UI_pipeData = new UserInterface(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::SIMPLEX_WR);
}



MainWorker::~MainWorker()
{
    delete this->UI_pipeData;
}





