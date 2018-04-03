#ifndef USERPACKSCONVERTER_H_
#define USERPACKSCONVERTER_H_


#include "UserPackFW.h"
#include "UserPackHL.h"


/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
class UserPacksConverter {
public:
	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	static UserPackHL ToHL(const UserPackFW &pack);

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	static std::vector<UserPackFW> ToFW(const UserPackHL &pack);

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	UserPacksConverter() = delete;
	~UserPacksConverter() = delete;
	UserPacksConverter(UserPacksConverter const&) = delete;
	UserPacksConverter& operator= (UserPacksConverter const&) = delete;
};

#endif /* USERPACKSCONVERTER_H_ */
