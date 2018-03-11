#ifndef COMHANDLER_H_
#define COMHANDLER_H_



#include "UserPack.h"
#include "COMPort.h"



class COMHandler {
public:
	enum class STATE {
		CLOSED = 0,
		OPENED
	};
	enum class RESULT {
		ERROR = -1,
		SUCCESS
	};

	COMHandler();
	uint8 GetPortNumber() const;
	COMHandler::STATE GetState() const;
	COMHandler::RESULT Send(const UserPack *pack) const;
	COMHandler::RESULT Receive(UserPack *pack) const;
private:
	COMPort port;
	uint8 portNumber;
	COMHandler::STATE state;
};



#endif /* COMHANDLER_H_ */
