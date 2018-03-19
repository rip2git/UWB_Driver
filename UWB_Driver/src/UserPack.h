#ifndef USERPACK_H_
#define USERPACK_H_


#include <stdint.h>


#define UserPack_DEBUG_MODE


/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
struct UserPack {
	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	enum class COMMAND : uint8_t {
		Status = 0,
		SetID,
		Distance,
		Data
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	enum class STATUS : uint8_t {
		Reserved = 0,
		SetID,
		Distance,
		Data
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	static const uint8_t BROADCAST_ID = 0xFF;
	static const uint8_t DATA_OFFSET = 3;
	static const uint8_t MAX_DATA_SIZE = 127; // according with IEEE Std 802.15.4-2011
	static const uint8_t MAX_PACK_BYTE = MAX_DATA_SIZE + DATA_OFFSET + 2;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	COMMAND Command;
	uint8_t DestinationID;
	uint8_t TotalSize;
	uint8_t Data[MAX_DATA_SIZE];

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	uint8_t ToBytes(uint8_t *buffer) const;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	void ToStruct(const uint8_t *buffer);

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	void SetData(const uint8_t *buffer);

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	void Reset();

#ifdef UserPack_DEBUG_MODE
	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	void Print() const;
#endif
};



#endif /* USERPACK_H_ */
