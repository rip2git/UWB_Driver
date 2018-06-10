
#include "MainWorker.h"

#include <thread>
#include <mutex>
#include <iostream>



void MainWorker::Start()
{
	this->UI_pipeData->Initialization();
	this->UI_pipeDBG->Initialization();
	this->dataThread = std::thread(&MainWorker::setData, this);
	this->dataThread.detach();
	this->debugThread = std::thread(&MainWorker::setDebug, this);
	this->debugThread.detach();


	this->nc->PrintDBG("treadsSTARTED");

	while(1)
		CrossSleep(60000);
}



void MainWorker::setData(void)
{
	UserPackHL upack;

	while (1) {
		if (this->UI_pipeData->Read( upack ) == UserInterface::RESULT::SUCCESS) {
			this->mu_rd.lock();
			switch (upack.FCmd.Cmd) {
				case UserPackHL::Command::Distance: {
					int dist = (upack.Data[6] + (upack.Data[7] << 8));
					this->tbl_Distance->SetValue(std::to_string(dist), 1, upack.SCmd - 1);
				} break;
				case UserPackHL::Command::Data: {
					std::string sTmp(upack.Data.begin(), upack.Data.end());
					std::string qsTmp = "id" + std::to_string(upack.SCmd) + ": " + sTmp;
					this->list_Receive->PushBack(qsTmp);
				} break;
				default: break; // nop
			}
			this->nc->Refresh();
			this->mu_rd.unlock();
		}
	}
}



void MainWorker::setDebug(void)
{
	UserPackHL upack;

	while (1) {
		if (this->UI_pipeDBG->Read( upack ) == UserInterface::RESULT::SUCCESS) {
			this->mu_rd.lock();
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
						case 0: {
							std::string qsTmp = "";
							for (int i = 2; i < upack.TotalSize; ++i) {
								if (upack.Data[1] == 'S')
									qsTmp += upack.Data[i];
								else
									;
//									qsTmp += QString("%1").arg(upack.Data[i], 2, 16, QChar('0')) + " ";
							}
							this->list_CommonLog->PushBack(qsTmp);
						} break;
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
						case 3: { // recv / ret state
							// [1] - state
							this->lbl_RecvDbg->SetData({"Recv: " + std::to_string(upack.Data[1])});
						} break;
						case 4: { // send state
							// [1] - state
							this->lbl_SendDbg->SetData({"Send: " + std::to_string(upack.Data[1])});
						} break;
					}
				} break;
				default: break; // nop
			}
			this->nc->Refresh();
			this->mu_rd.unlock();
		}
	}
}



MainWorker::MainWorker()
{
	this->nc = new NCurses();

	this->nc->PrintDBG("started");

    this->UI_pipeData = new UserInterface(UserInterface::MODE::OPEN_EXISTING, UserInterface::CONNTYPE::SIMPLEX_RD);
    this->UI_pipeDBG = new UserInterfaceDBG(UserInterfaceDBG::MODE::OPEN_EXISTING, UserInterfaceDBG::CONNTYPE::SIMPLEX_RD);

    this->tbl_Distance = new NCursesTable();
    this->tbl_Levels = new NCursesTable();
    this->tbl_Routing = new NCursesTable();

    this->list_Receive = new NCursesList();
    this->list_CommonLog = new NCursesList();
    this->lbl_RecvDbg = new NCursesList();
    this->lbl_SendDbg = new NCursesList();


	{ // register
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->tbl_Distance) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->tbl_Levels) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->tbl_Routing) );

    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->list_Receive) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->list_CommonLog) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->lbl_RecvDbg) );
    	this->nc->RegisterObject( dynamic_cast<NCursesObject *>(this->lbl_SendDbg) );
	}

	{ // titles
		this->tbl_Distance->SetTitle("Distance");
		this->tbl_Levels->SetTitle("Levels");
		this->tbl_Routing->SetTitle("Routing");
		this->lbl_RecvDbg->isTitled = false;
		this->lbl_SendDbg->isTitled = false;
		this->list_Receive->isTitled = true;
		this->list_CommonLog->isTitled = true;
		this->list_Receive->SetTitle("Received");
		this->list_CommonLog->SetTitle("Common log");
	}

	{ // positions
		this->tbl_Distance->SetPosition(0, 1);
		this->tbl_Levels->SetPosition(0, 20);
		this->tbl_Routing->SetPosition(0, 35);
		this->list_Receive->SetPosition(17, 1);
		this->lbl_RecvDbg->SetPosition(0, 50);
		this->lbl_SendDbg->SetPosition(3, 50);
		this->list_CommonLog->SetPosition(28, 1);
	}

	{ // list sizes
		this->list_Receive->Resize(58, 6);
		this->lbl_RecvDbg->Resize(10, 1);
		this->lbl_SendDbg->Resize(10, 1);
		this->list_CommonLog->Resize(58, 6);
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
		this->list_Receive->PushBack(" ");
		this->lbl_RecvDbg->PushBack(" ");
		this->lbl_SendDbg->PushBack(" ");
		this->list_CommonLog->PushBack(" ");
	}

	this->nc->PrintDBG("initOK");
	this->nc->Refresh();
}



MainWorker::~MainWorker()
{
    delete this->nc;
    delete this->UI_pipeData;
    delete this->UI_pipeDBG;
}





