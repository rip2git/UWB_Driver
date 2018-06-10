
#include "MainWorker.h"

#include <thread>
#include <mutex>
#include <iostream>



void MainWorker::Start()
{
	this->UI_pipeData->Initialization();
	this->UI_pipeDBG->Initialization();

	this->muRD = new std::mutex();
	this->muWR = new std::mutex();

	CrossSleep(1000);

	this->timer->start(0);

	this->debugRDThread = new std::thread(&MainWorker::setDebug, this);
	this->debugRDThread->detach();
	this->dataRDThread = new std::thread(&MainWorker::setData, this);
	this->dataRDThread->detach();

	CrossSleep(10);

	this->distanceWRThread = new std::thread(&MainWorker::sendDistance, this);
	this->distanceWRThread->detach();
	this->dataWRThread = new std::thread(&MainWorker::sendData, this);
	this->dataWRThread->detach();

	this->nc->PrintDBG("treadsSTARTED");

	while(1)
		CrossSleep(60000);
}



void MainWorker::setDebug(void)
{
	UserPackHL upack;

	while (1) {
		if (this->UI_pipeDBG->Read( upack ) == UserInterface::RESULT::SUCCESS) {
//			this->muRD->lock();
			switch (upack.FCmd.Cmd) {
				case UserPackHL::Command::Service: {
					/*
					 * [0] --> Debug group:
					 *      =0 - common log, [1] = 'S' - symbolic, '0' - raw;
					 *      =1 - routing table: lvls x10, ids(-1) x10;
					 *      =2 - routing levels: id, lvl
					 *      =3 - recv/ret state
					 *      =4 - send state
					*/
					switch (upack.Data[0]) {
						case 1: { // routing tables
							// ids-lvls;
							for (int i = 0; i < 10; ++i) {
								this->tbl_Routing->SetValue(std::to_string(upack.Data[i + 11] + 1), 0, i);
								this->tbl_Routing->SetValue(std::to_string(upack.Data[i + 1]), 1, i);
							}
						} break;
						case 2: { // routing levels
							// [1] - id, [2] - level
							this->tbl_Levels->SetValue(std::to_string(upack.Data[2]), 1, upack.Data[1]-1);
						} break;
					}
				} break;
				default: break; // nop
			}
			this->nc->Refresh();
//			this->muRD->unlock();
		}
	}
}



void MainWorker::setData(void)
{
	UserPackHL upack;
	uint16_t cnt_rx = 0;
	uint16_t tmp;

	while (1) {
		if (this->UI_pipeData->Read( upack ) == UserInterface::RESULT::SUCCESS) {
//			this->muRD->lock();
			switch (upack.FCmd.Cmd) {
				case UserPackHL::Command::Data: {
					cnt_rx++;
					tmp = upack.Data[0] | (upack.Data[1] << 8);
					this->lbl_CntRx->SetData({"CntRx: "
						+ std::to_string(cnt_rx) + "/" + std::to_string(tmp)
						+ " - lost: " + std::to_string(tmp-cnt_rx) + "              "});
				} break;
				case UserPackHL::Command::Distance: {
					int dist = (upack.Data[6] + (upack.Data[7] << 8));
					this->tbl_Distance->SetValue(std::to_string(dist), 1, upack.SCmd - 1);
				} break;
				default: break; // nop
			}
			this->nc->Refresh();
//			this->muRD->unlock();
		}
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

//	while (1) {
//		this->muWR->lock();
//		this->UI_pipeData->Write(dupack);
//		CrossSleep(50);
//		this->muWR->unlock();
//		CrossSleep(450);
//	}
}



void MainWorker::sendData(void)
{
	UserPackHL upack;
	uint16_t cnt_tx = 0;

	upack.FCmd = UserPackHL::Command::Data;
	upack.SCmd = this->toID;
	upack.TotalSize = 110;
	upack.Data.resize(upack.TotalSize);
	uint8_t val = 'd';
	std::fill(upack.Data.begin(), upack.Data.end(), val);

	while (1) {
		this->muWR->lock();
		cnt_tx++;
		upack.Data[0] = static_cast<uint8_t>(cnt_tx);
		upack.Data[1] = static_cast<uint8_t>(cnt_tx >> 8);
		this->UI_pipeData->Write(upack);
		this->lbl_CntTx->SetData({"CntTx: " + std::to_string(cnt_tx) + "            "});
		CrossSleep(50);
		this->muWR->unlock();
		CrossSleep(450);
	}
}



MainWorker::MainWorker()
{
	std::cout << "To id (1-10): " << std::endl;
	std::cin >> this->toID;
	this->toID &= 0x0F;

	this->nc = new NCurses();

	this->nc->PrintDBG("started");

    this->UI_pipeData = new UserInterface(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::DUPLEX);
    this->UI_pipeDBG = new UserInterfaceDBG(UserInterfaceDBG::MODE::OPEN_EXISTING, UserInterfaceDBG::CONNTYPE::SIMPLEX_RD);

    this->tbl_Distance = new NCursesTable();
    this->tbl_Levels = new NCursesTable();
    this->tbl_Routing = new NCursesTable();

    this->lbl_CntRx = new NCursesList();
    this->lbl_CntTx = new NCursesList();


	{ // register
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->tbl_Distance) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->tbl_Levels) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->tbl_Routing) );

    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->lbl_CntRx) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->lbl_CntTx) );
	}

	{ // titles
		this->tbl_Distance->SetTitle("Distance");
		this->tbl_Levels->SetTitle("Levels");
		this->tbl_Routing->SetTitle("Routing");
		this->lbl_CntRx->isTitled = false;
		this->lbl_CntTx->isTitled = false;
	}

	{ // positions
		this->tbl_Distance->SetPosition(0, 1);
		this->tbl_Levels->SetPosition(0, 20);
		this->tbl_Routing->SetPosition(0, 35);
		this->lbl_CntRx->SetPosition(17, 1);
		this->lbl_CntTx->SetPosition(21, 1);
	}

	{ // list sizes
		this->lbl_CntRx->Resize(45, 1);
		this->lbl_CntTx->Resize(45, 1);
	}

	{ // tables fill
		std::vector < std::vector <std::string> > cv = {
				{"1", "2", "3", "4", "5", "6", "7", "8" ,"9", "10"},
				{" ", " ", " ", " ", " ", " ", " ", " " ," ", " "}
		};
		std::vector <std::string> cn = {"ids", ""};
		cn[1] = "distance";
		this->tbl_Distance->SetData(cv, cn);
		cn[1] = "lvls";
		this->tbl_Levels->SetData(cv, cn);
		cn[1] = "hops";
		this->tbl_Routing->SetData(cv, cn);
	}

	{ // lists fill
		this->lbl_CntRx->PushBack("CntRx: 0/0 - lost: 0");
		this->lbl_CntTx->PushBack("CntTx: 0");
	}

	this->nc->PrintDBG("initOK");
	this->nc->Refresh();

    this->debugRDThread = nullptr;
    this->dataRDThread = nullptr;
    this->dataWRThread = nullptr;
    this->distanceWRThread = nullptr;
    this->muRD = nullptr;
    this->muWR = nullptr;

    this->timer = new TON();
}



MainWorker::~MainWorker()
{
    delete this->nc;
    delete this->UI_pipeData;
    delete this->UI_pipeDBG;
    delete this->debugRDThread;
    delete this->dataRDThread;
    delete this->dataWRThread;
    delete this->distanceWRThread;
    delete this->muRD;
    delete this->muWR;
    delete this->timer;
}





