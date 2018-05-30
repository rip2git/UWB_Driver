#ifndef MAINWORKER_H_
#define MAINWORKER_H_

#include "CrossSleep.h"
#include "UserInterface.h"
#include "UserInterfaceDBG.h"
#include "NCurses.h"

class MainWorker {
public:
	MainWorker();
	~MainWorker();
	void Start();
private:
	NCurses *nc;
	NCursesTable *tbl_Distance;
	NCursesTable *tbl_Levels;
	NCursesTable *tbl_Routing;

	NCursesList *list_Receive;
	NCursesList *list_CommonLog;
	NCursesList *lbl_RecvDbg;
	NCursesList *lbl_SendDbg;

	UserInterface *UI_pipeData;
	UserInterfaceDBG *UI_pipeDBG;

	std::thread dataThread;
	std::thread debugThread;
	std::mutex mu_rd;

    void setData(void);
    void setDebug(void);
};

#endif /* MAINWORKER_H_ */
