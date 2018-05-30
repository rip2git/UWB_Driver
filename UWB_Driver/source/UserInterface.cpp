
#include "UserInterface.h"
#include "CrossSleep.h"
#include "IniFiles.h"
#include "CFG.h"



UserInterface::UserInterface(UserInterface::MODE mode, UserInterface::CONNTYPE connectionType)
{
	this->state = UserInterface::STATE::CLOSED;
	this->mode = mode;
	this->connectionType = connectionType;
}



void UserInterface::Initialization()
{
	if (this->state == UserInterface::STATE::CLOSED) {
		NamedPipe::InitializationStruct init_str;

		bool bWR = this->connectionType == UserInterface::CONNTYPE::DUPLEX || this->connectionType == UserInterface::CONNTYPE::SIMPLEX_WR;
		bool bRD = this->connectionType == UserInterface::CONNTYPE::DUPLEX || this->connectionType == UserInterface::CONNTYPE::SIMPLEX_RD;

		try {
			std::map <std::string, std::string> ucnf;
			IniFile ini(CFG::FILE_NAME);

			if (ini.isOpened())
				ini.Parse();
			else
				throw std::exception();

			ucnf = ini.GetSection(CFG::PIPE::SECTION);

			if (bWR) {
				if ( ucnf.find(CFG::PIPE::WRP_NAME) == ucnf.end() ) throw std::exception();
				init_str.pipeName.assign(ucnf.find(CFG::PIPE::WRP_NAME)->second);
				init_str.mode = NamedPipe::MODE::WRITE;
				this->wr_pipe.Initialization(init_str);
			}
			if (bRD) {
				if ( ucnf.find(CFG::PIPE::RDP_NAME) == ucnf.end() ) throw std::exception();
				init_str.pipeName.assign(ucnf.find(CFG::PIPE::RDP_NAME)->second);
				init_str.mode = NamedPipe::MODE::READ;
				this->rd_pipe.Initialization(init_str);
			}

		} catch (std::exception &e) {
			return;
		}

		if (this->mode == UserInterface::MODE::CREATE_NEW) {
			if (bWR) {
				this->wr_thr = std::thread(&UserInterface::wrCreate, this);
				this->wr_thr.detach();
			}
			if (bRD) {
				this->rd_thr = std::thread(&UserInterface::rdCreate, this);
				this->rd_thr.detach();
			}
			CrossSleep(10);
			if (bWR && this->wr_pipe.GetState() == NamedPipe::STATE::INITIALIZED)
				this->wr_mutex.try_lock();
			if (bRD && this->rd_pipe.GetState() == NamedPipe::STATE::INITIALIZED)
				this->rd_mutex.try_lock();
			this->state = UserInterface::STATE::OPENED;
		} else { // UserInterface::OPEN_EXISTING
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
			this->state = UserInterface::STATE::OPENED;
		}
		return;
	}
}



UserInterface::~UserInterface()
{
	this->Close();
}



void UserInterface::Close()
{
	if (this->state != UserInterface::STATE::CLOSED) {
		this->state = UserInterface::STATE::CLOSED;
		this->wr_pipe.Close();
		this->rd_pipe.Close();
		this->rd_mutex.unlock();
		this->wr_mutex.unlock();
	}
	return;
}



bool UserInterface::CheckWorkingCapacity()
{
	return this->rd_pipe.CheckWorkingCapacity();
}



void UserInterface::ReOpen()
{
	this->Close();
	this->Initialization();
	return;
}



UserInterface::RESULT UserInterface::Write(const UserPackHL &pack)
{
    if (this->state == UserInterface::STATE::OPENED &&
       (this->connectionType == UserInterface::CONNTYPE::DUPLEX ||
        this->connectionType == UserInterface::CONNTYPE::SIMPLEX_WR)
    ) {
		std::vector <uint8_t> buffer;
		pack.ToBytes(buffer);

		int res = this->wr_pipe.Write( &(buffer[0]), buffer.size() );

		if (res == -1) {
			this->ReOpen();
			return UserInterface::RESULT::ERROR;
		}

		return UserInterface::RESULT::SUCCESS;
	}
	return UserInterface::RESULT::ERROR;
}



UserInterface::RESULT UserInterface::Read(UserPackHL &pack)
{
    if (this->state == UserInterface::STATE::OPENED &&
       (this->connectionType == UserInterface::CONNTYPE::DUPLEX ||
        this->connectionType == UserInterface::CONNTYPE::SIMPLEX_RD)
    ) {
		std::vector <uint8_t> buffer;
		buffer.resize(UserPackHL::DATA_OFFSET);

		pack.Reset();

		int res = this->rd_pipe.Read( &(buffer[0]), UserPackHL::DATA_OFFSET );

		if (res == -1) {
			this->ReOpen();
			return UserInterface::RESULT::ERROR;
		}

		pack.TotalSize = buffer[2];
		pack.TotalSize |= buffer[3] << 8;
		buffer.resize(UserPackHL::DATA_OFFSET + pack.TotalSize); // saves content

		res = this->rd_pipe.Read( &(buffer[UserPackHL::DATA_OFFSET]), pack.TotalSize );

		if (res == -1) {
			this->ReOpen();
			return UserInterface::RESULT::ERROR;
		}

		pack.ToStruct(buffer);

		return UserInterface::RESULT::SUCCESS;
	}
	return UserInterface::RESULT::ERROR;
}



UserInterface::STATE UserInterface::GetState() const
{
	return this->state;
}



void UserInterface::rdCreate()
{
	this->rd_mutex.try_lock();
	this->rd_pipe.Create();
	this->rd_mutex.unlock();
}



void UserInterface::wrCreate()
{
	this->wr_mutex.try_lock();
	this->wr_pipe.Create();
	this->wr_mutex.unlock();
}

