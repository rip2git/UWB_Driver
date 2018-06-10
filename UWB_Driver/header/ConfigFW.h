#ifndef CONFIGFW_H_
#define CONFIGFW_H_


#include "UserPackFW.h"



/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
class ConfigFW {
public:
	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	enum class STATE {
		ERROR = -1,
		AVAILABLE
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	void ToUserPackFW(UserPackFW &pack) const;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	ConfigFW();

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	ConfigFW::STATE GetState() const;

private:

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	const uint8_t configSize = 14;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	struct SW1000_Str {
		uint8_t		DebugMode;
		uint16_t 	DeviceID;
		uint16_t 	PAN_ID;
		uint8_t		nDevices;
		uint16_t	PollingPeriod;
	} SW1000;
	struct Token_Str {
		uint8_t		TimeSlotDurationMs;
	} Token;
	struct Routing_Str {
		uint8_t		MinSignalLevel;
	} Routing;
	struct Ranging_Str {
		uint16_t	RespondingDelay;
		uint16_t	FinalDelay;
	} Ranging;


	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	ConfigFW::STATE state;
};

#endif /* CONFIGFW_H_ */
