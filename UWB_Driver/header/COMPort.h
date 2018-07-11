#ifndef COMPort_H
#define COMPort_H


#include <stdint.h>
#include <string>


#ifdef __linux__
	typedef uint32_t DWORD;
	typedef int HANDLE;
#else
	typedef unsigned long DWORD;
	typedef void* HANDLE;
#endif



/*! ----------------------------------------------------------------------------------------
 * @brief: Interface between driver and hard devices
 * -----------------------------------------------------------------------------------------
 * */
class COMPort {
public:
	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	enum class STATE {
		CLOSED = 0,
		OPENED
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	struct TimeOutStruct {
		DWORD Ms;
		DWORD nChars;
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	struct InitializationStruct {
		std::string portName;
		DWORD baudRate;
		TimeOutStruct timeOut;
	};

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	COMPort();
	COMPort(const InitializationStruct &initStr);

	/*! ------------------------------------------------------------------------------------
	 * @brief: Calls Close
	 * -------------------------------------------------------------------------------------
	 * */
	~COMPort();

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	COMPort::STATE GetState() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Closes file descriptor
	 * -------------------------------------------------------------------------------------
	 * */
	void Close();

	/*! ------------------------------------------------------------------------------------
	 * @brief: Flush by OS abilities
	 * -------------------------------------------------------------------------------------
	 * */
	bool FastFlush() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Slow flush: doing fast flush, gets delay = charsSpacing and tries to read
	 * byte. If last is failed when return true, else repeats from the beginning.
	 * Changes timeOut for its own purposes. Returns timeOut before return from call
	 * -------------------------------------------------------------------------------------
	 * */
	void Flush() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Returns current time out
	 * -------------------------------------------------------------------------------------
	 * */
	const TimeOutStruct& GetCurrentTimeOut() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	int GetAvailableBytesOfRecvBuf() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Sets timeOut and calls ChangeTimeOut
	 * -------------------------------------------------------------------------------------
	 * */
	bool SetTimeOut(const TimeOutStruct &timeOut);

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	void Initialization(const InitializationStruct &initStr);

	/*! ------------------------------------------------------------------------------------
	 * @brief: Reads data from buffer (behavior is dependent on parameters of ChangeTimeOut);
	 * Returns received bytes (0:n) or failure (-1)
	 * -------------------------------------------------------------------------------------
	 * */
	int Read(uint8_t *buffer, uint8_t buffer_size) const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Reads one byte from buffer (behavior is dependent on parameters of
	 * ChangeTimeOut);
	 * Returns received bytes (0:1) or failure (-1)
	 * -------------------------------------------------------------------------------------
	 * */
	int ReadByte(uint8_t *buffer) const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Writes data to buffer (up to 256 bytes per write) / transfers data as is
	 * Returns transfered (n) bytes or failure (-1)
	 * -------------------------------------------------------------------------------------
	 * */
	int Write(const uint8_t *buffer, uint8_t buffer_size) const;

private:
	/*! ------------------------------------------------------------------------------------
	 * @brief: File descriptor / used by OS
	 * -------------------------------------------------------------------------------------
	 * */
	HANDLE fd;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	COMPort::STATE state;

	/*! ------------------------------------------------------------------------------------
	 * @brief:
	 * -------------------------------------------------------------------------------------
	 * */
	DWORD baudRate;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Current time out
	 * -------------------------------------------------------------------------------------
	 * */
	TimeOutStruct timeOut;

	/*! ------------------------------------------------------------------------------------
	 * @brief: length of 10 bytes in ms
	 * -------------------------------------------------------------------------------------
	 * */
	uint8_t charsSpacing;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Checks port for system errors / close if it was detected
	 * -------------------------------------------------------------------------------------
	 * */
	bool GetStatus() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Resets errno
	 * -------------------------------------------------------------------------------------
	 * */
	int GetLastSystemError() const;

	/*! ------------------------------------------------------------------------------------
	 * @brief: Changes the algorithm for receiving data:
	 * User sets time out to read data and number of chars which should be received;
	 * Reset errno
	 * -------------------------------------------------------------------------------------
	 * */
	bool ChangeTimeOut(const TimeOutStruct &timeOut) const;
};


#endif
