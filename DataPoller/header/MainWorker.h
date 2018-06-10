#ifndef MAINWORKER_H_
#define MAINWORKER_H_

#include "CrossSleep.h"
#include "UserInterface.h"
#include "UserInterfaceDBG.h"
#include "NCurses.h"
#include "TON.h"

class MainWorker {
public:
	MainWorker();
	~MainWorker();
	void Start();
private:
	int toID;

	TON *timer;

	NCurses *nc;
	NCursesTable *tbl_Distance;
	NCursesTable *tbl_Levels;
	NCursesTable *tbl_Routing;

	NCursesList *lbl_CntRx;
	NCursesList *lbl_CntTx;

	UserInterface *UI_pipeData;
	UserInterfaceDBG *UI_pipeDBG;

	std::thread *debugRDThread;
	std::thread *dataRDThread;
	std::thread *dataWRThread;
	std::thread *distanceWRThread;

	std::mutex *muRD;
	std::mutex *muWR;

    void setDebug(void);
    void setData(void);
    void sendData(void);
    void sendDistance(void);
};

#endif /* MAINWORKER_H_ */
