#ifndef MAINWORKERR_H_
#define MAINWORKERR_H_

#include "CrossSleep.h"
#include "UserInterface.h"

class MainWorker {
public:
	MainWorker();
	~MainWorker();
	void Start();
private:
	UserInterface *UI_pipeData;
	UserPackHL UPackData;
	std::thread dataThread;
	std::thread distanceThread;
	std::mutex mu;
	bool send_data_fl = false;

    void sendData(void);
    void sendDistance(void);
};

#endif /* MAINWORKERR_H_ */
