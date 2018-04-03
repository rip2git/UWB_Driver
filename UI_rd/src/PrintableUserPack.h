#ifndef PRINTABLEUSERPACK_H_
#define PRINTABLEUSERPACK_H_


#include "UserPackHL.h"



class PrintableUserPack : public UserPackHL {
public:
	void Print(std::ostream &os) const;
};


#endif /* PRINTABLEUSERPACK_H_ */
