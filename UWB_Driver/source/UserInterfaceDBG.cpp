
#include "UserInterfaceDBG.h"
#include "CrossSleep.h"
#include "IniFiles.h"
#include "CFG.h"



void UserInterfaceDBG::Initialization()
{
	if (this->state == UserInterfaceDBG::STATE::CLOSED) {
		NamedPipe::InitializationStruct init_str;

		bool bWR = this->connectionType == UserInterfaceDBG::CONNTYPE::DUPLEX || this->connectionType == UserInterfaceDBG::CONNTYPE::SIMPLEX_WR;
		bool bRD = this->connectionType == UserInterfaceDBG::CONNTYPE::DUPLEX || this->connectionType == UserInterfaceDBG::CONNTYPE::SIMPLEX_RD;

		try {
			std::map <std::string, std::string> ucnf;
			IniFile ini(CFG::FILE_NAME);

			if (ini.isOpened())
				ini.Parse();
			else
				throw std::exception();

			ucnf = ini.GetSection(CFG::DBG_PIPE::SECTION);

			/*if ( ucnf.find(CFG::DBG_PIPE::WRP_NAME) == ucnf.end() ) throw std::exception();
			std::string enable = ucnf.find(CFG::DBG_PIPE::ENABLE)->second;
			if (enable == "0" || enable == "off")
				throw std::exception();*/

			if (bWR) {
				if ( ucnf.find(CFG::DBG_PIPE::WRP_NAME) == ucnf.end() ) throw std::exception();
				init_str.pipeName.assign(ucnf.find(CFG::DBG_PIPE::WRP_NAME)->second);
				init_str.mode = NamedPipe::MODE::WRITE;
				this->wr_pipe.Initialization(init_str);
			}
			if (bRD) {
				if ( ucnf.find(CFG::DBG_PIPE::RDP_NAME) == ucnf.end() ) throw std::exception();
				init_str.pipeName.assign(ucnf.find(CFG::DBG_PIPE::RDP_NAME)->second);
				init_str.mode = NamedPipe::MODE::READ;
				this->rd_pipe.Initialization(init_str);
			}

		} catch (std::exception &e) {
			return;
		}

		if (this->mode == UserInterfaceDBG::MODE::CREATE_NEW) {
			if (bWR) {
				this->wr_thr = std::thread(&UserInterfaceDBG::wrCreate, this);
				this->wr_thr.detach();
			}
			if (bRD) {
				this->rd_thr = std::thread(&UserInterfaceDBG::rdCreate, this);
				this->rd_thr.detach();
			}
			CrossSleep(10);
			if (bWR && this->wr_pipe.GetState() == NamedPipe::STATE::INITIALIZED)
				this->wr_mutex.lock();
			if (bRD && this->rd_pipe.GetState() == NamedPipe::STATE::INITIALIZED)
				this->rd_mutex.lock();
			this->state = UserInterfaceDBG::STATE::OPENED;
        } else { // UserInterfaceDBG::OPEN_EXISTING
			while (1) {
				if (bWR)
					this->wr_pipe.Open();
				if (bRD)
					this->rd_pipe.Open();
				if (bWR && bRD) {
					if (this->wr_pipe.GetState() == NamedPipe::STATE::OPENED &&
						this->rd_pipe.GetState() == NamedPipe::STATE::OPENED
					) {
						break;
					}
				} else if (bWR) {
					if (this->wr_pipe.GetState() == NamedPipe::STATE::OPENED)
						break;
				} else if (bRD) {
					if (this->rd_pipe.GetState() == NamedPipe::STATE::OPENED)
						break;
				}
				CrossSleep(200);
			}
			this->state = UserInterfaceDBG::STATE::OPENED;
		}
		return;
	}
}




UserInterfaceDBG::RESULT UserInterfaceDBG::Write(const UserPackHL &pack)
{
    if (this->state == UserInterfaceDBG::STATE::OPENED &&
       (this->connectionType == UserInterfaceDBG::CONNTYPE::DUPLEX ||
        this->connectionType == UserInterfaceDBG::CONNTYPE::SIMPLEX_WR)
    ) {
        std::vector <uint8_t> buffer;
        pack.ToBytes(buffer);

        return (this->wr_pipe.Write( &(buffer[0]), buffer.size() ) < 0)?
                UserInterfaceDBG::RESULT::ERROR : UserInterfaceDBG::RESULT::SUCCESS;
    }
    return UserInterfaceDBG::RESULT::ERROR;
}




UserInterfaceDBG::~UserInterfaceDBG()
{
	this->Close();
}
