#ifndef CFG_H_
#define CFG_H_


namespace CFG {

	const char* const FILE_NAME 			= "config.ini";

	namespace MAIN {
		const char* const SECTION 			= "MAIN_SECTION";
		const char* const LOG_MODE 			= "log_mode";
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

    namespace DBG_PIPE {
        const char* const SECTION			= "DBG_PIPE_SECTION";
        const char* const ENABLE 			= "dbg_pipe_enable";
        const char* const WRP_NAME 			= "dbg_wr_name";
        const char* const RDP_NAME 			= "dbg_rd_name";
    };

    namespace DATA {
		const char* const SECTION			= "DATA_SECTION";
		const char* const BUFFER_SIZE		= "buffer_size";
		const char* const TRANSACTION_SIZE	= "transaction_size";
		const char* const TRUST_PACKS		= "trust_packs";
		const char* const REPEATS			= "repeats";
	};

	namespace FIRMWARE {
		const char* const SECTION			= "FIRMWARE_SECTION";
		const char* const DEVICE_ID 		= "device_id";
		const char* const PAN_ID 			= "pan_id";
		const char* const NUM_DEVICES		= "n_devices";
		const char* const POLL_PERIOD		= "polling_period";
		const char* const TOKEN_TSL_DUR		= "tok_timeslot_duration";
		const char* const RESP_DELAY		= "response_delay";
		const char* const FINAL_DELAY		= "final_delay";
	};
};


#endif /* CFG_H_ */
