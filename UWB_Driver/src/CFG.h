#ifndef CFG_H_
#define CFG_H_


namespace CFG {

	const char* const FILE_NAME 			= "config.ini";

	namespace MAIN {
		const char* const SECTION 			= "MAIN_SECTION";
		const char* const LOG_MODE 			= "log_mode";
		const char* const DEVICE_ID 		= "device_id";
	};

	namespace COM {
		const char* const SECTION 			= "COM_SECTION";
		const char* const COM_NAME 			= "name";
		const char* const BAUD_RATE 		= "baud_rate";
	};

	namespace PIPE {
		const char* const SECTION			= "PIPE_SECTION";
		const char* const WRP_NAME 			= "wr_name";
		const char* const RDP_NAME 			= "rd_name";
	};
};


#endif /* CFG_H_ */
