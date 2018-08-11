#ifndef USERPACK_H_
#define USERPACK_H_


#include "_DEBUG.h"
#include <stdint.h>
#include <vector>


#ifdef UserPack_DEBUG_MODE
#include <fstream>
#endif



/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
struct UserPack {
	/*! ------------------------------------------------------------------------------------
	 * @brief: Commands from user to firmware
	 * -------------------------------------------------------------------------------------
	 * */
	enum class Command : uint8_t {
		Service = 0x80,
		SystemConfig = 0x81,
		Distance = 0x82,
		DataConfig = 0x83,
		Data = 0x84,
		Ack = 0x85,
		End = 0x86,
		Status = 0x87
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief: Results of commands from user to firmware
	 * -------------------------------------------------------------------------------------
	 * */
	enum class CommandResult : uint8_t {
		Fail = 0x40,
		Success = 0x41,
		Accepted = 0x42
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	union FCommand {
		uint8_t _raw;
		CommandResult Res;
		Command Cmd;

		FCommand& operator=(Command cmd) {
			Cmd = cmd;
			return *this;
		}
		FCommand& operator=(CommandResult cmdres) {
			Res = cmdres;
			return *this;
		}
		FCommand& operator=(uint8_t raw) {
			_raw = raw;
			return *this;
		}
		bool operator==(const FCommand& fcmd) {
			return (this->_raw == fcmd._raw);
		}
		operator CommandResult() const {
			return Res;
		}
		operator uint8_t() const {
			return _raw;
		}
		operator Command() const {
			return Cmd;
		}
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	union SCommand {
		uint8_t _raw;
		uint8_t DeviceID;
		Command Cmd;

		SCommand& operator=(uint8_t id) {
			DeviceID = id;
			return *this;
		}
		SCommand& operator=(Command cmd) {
			Cmd = cmd;
			return *this;
		}
		bool operator==(const SCommand& scmd) {
			return (this->_raw == scmd._raw);
		}
		operator uint8_t() const {
			return _raw;
		}
		operator Command() const {
			return Cmd;
		}
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	static const uint8_t BROADCAST_ID = 0xFF;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	FCommand FCmd;
	SCommand SCmd;
	std::vector <uint8_t> Data;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	static bool isCommand(const FCommand &Fcmd);
	static bool isResult(const FCommand &FCmd);
	static bool isCommand(const SCommand &Scmd);
	static bool isDeviceID(const SCommand &Scmd);

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	virtual void ToBytes(std::vector <uint8_t> &buffer) const = 0;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	virtual void ToStruct(const std::vector <uint8_t> &buffer) = 0;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	virtual void SetData(const std::vector <uint8_t> &buffer) = 0;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	virtual void Reset() = 0;

#ifdef UserPack_DEBUG_MODE
	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	virtual void Print(std::ostream &os) const = 0;
#endif

	virtual ~UserPack() {};
};



#endif /* USERPACK_H_ */
