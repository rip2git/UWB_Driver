#include "UserInterface.h"
#include "CrossSleep.h"
#include "IniFiles.h"
#include "CFG.h"




UserInterface::UserInterface(UserInterface::MODE mode)
{
	this->mode = mode;
	this->Initialization();
}



void UserInterface::Initialization()
{
	NamedPipe::InitializationStruct init_str;

	try {
		std::map <std::string, std::string> ucnf;
		IniFile ini(CFG::FILE_NAME);

		if (ini.isOpened())
			ini.Parse();
		else
			throw std::exception();

		ucnf = ini.GetSection(CFG::COM::SECTION);

		if ( ucnf.find(CFG::PIPE::WRP_NAME) == ucnf.end() ) throw std::exception();
		if ( ucnf.find(CFG::PIPE::RDP_NAME) == ucnf.end() ) throw std::exception();

		init_str.pipeName.assign(ucnf.find(CFG::PIPE::WRP_NAME)->second);
		init_str.mode = NamedPipe::MODE::WRITE;
		this->rd_pipe.Initialization(&init_str);

		init_str.pipeName.assign(ucnf.find(CFG::PIPE::RDP_NAME)->second);
		init_str.mode = NamedPipe::MODE::READ;
		this->wr_pipe.Initialization(&init_str);

	} catch (std::exception &e) {
		;
	}


	if (this->mode == UserInterface::MODE::CREATE_NEW) {
		this->wr_thr = std::thread(&UserInterface::wrCreate, this);
		this->wr_thr.detach();
		this->rd_thr = std::thread(&UserInterface::rdCreate, this);
		this->rd_thr.detach();
		CrossSleep(10);
		if (this->wr_pipe.GetState() == NamedPipe::STATE::OPENED &&
			this->rd_pipe.GetState() == NamedPipe::STATE::OPENED
		) {
			this->Wait();
		}
	} else { // UserInterface::OPEN_EXISTING
		while (1) {
			this->wr_pipe.Open();
			this->rd_pipe.Open();
			if (this->wr_pipe.GetState() == NamedPipe::STATE::OPENED &&
				this->rd_pipe.GetState() == NamedPipe::STATE::OPENED
			) {
				break;
			}
			CrossSleep(2000);
		}
	}
	return;
}



UserInterface::~UserInterface()
{
	this->Close();
}



void UserInterface::Close()
{
	this->wr_pipe.Close();
	this->rd_pipe.Close();
	this->rd_mutex.unlock();
	this->wr_mutex.unlock();
	return;
}



void UserInterface::Wait() {
	this->rd_mutex.lock();
	this->wr_mutex.lock();
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



UserInterface::RESULT UserInterface::Write(const UserPackHL *pack)
{
	std::vector <uint8_t> buffer;
	pack->ToBytes(buffer);

	int res = this->wr_pipe.Write(&(buffer[0]), buffer.size());

	if (res == -1) {
		this->ReOpen();
		return UserInterface::RESULT::ERROR;
	}

	return UserInterface::RESULT::SUCCESS;
}



UserInterface::RESULT UserInterface::Read(UserPackHL *pack)
{
	std::vector <uint8_t> buffer;
	buffer.resize(UserPackHL::DATA_OFFSET);

	pack->Reset();

	int res = this->rd_pipe.Read(&(buffer[0]), UserPackHL::DATA_OFFSET);

	if (res == -1) {
		this->ReOpen();
		return UserInterface::RESULT::ERROR;
	}

	pack->TotalSize = buffer[2];
	pack->TotalSize |= buffer[3] << 8;
	buffer.resize(UserPackHL::DATA_OFFSET + pack->TotalSize); // saves content

	res = this->rd_pipe.Read( &(buffer[UserPackHL::DATA_OFFSET]), pack->TotalSize );

	if (res == -1) {
		this->ReOpen();
		return UserInterface::RESULT::ERROR;
	}

	return UserInterface::RESULT::SUCCESS;
}



void UserInterface::rdCreate()
{
	this->rd_mutex.lock();
	this->rd_pipe.Create();
	this->rd_mutex.unlock();
}



void UserInterface::wrCreate()
{
	this->wr_mutex.lock();
	this->wr_pipe.Create();
	this->wr_mutex.unlock();
}

