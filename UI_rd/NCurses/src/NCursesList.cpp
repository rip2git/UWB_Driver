
#include "NCursesList.h"



void NCursesList::SetData(const std::vector <std::string> &data)
{
	this->data.clear();
	if (data.size() > this->BUFFER_MAX_SIZE) {
		this->data.assign(data.begin() + (data.size() - this->BUFFER_MAX_SIZE), data.end());
	} else {
		this->data.assign(data.begin(), data.end());
	}
	std::vector <std::string> _data;
	if (this->data.size() > this->lines) {
		_data.assign(this->data.begin() + (this->data.size() - this->lines), this->data.end());
	} else {
		_data.assign(this->data.begin(), this->data.end());
	}
	NCursesListFrame::SetData(this->title, _data, str_to_string);
}



void NCursesList::PushBack(const std::string &str)
{
	this->data.push_back(str);
	std::vector <std::string> _data;
	if (this->data.size() > this->lines) {
		_data.assign(this->data.begin() + (this->data.size() - this->lines), this->data.end());
	} else {
		_data.assign(this->data.begin(), this->data.end());
	}
	NCursesListFrame::SetData(this->title, _data, str_to_string);
}



void NCursesList::Refresh()
{
	NCursesListFrame::Refresh();
}



void NCursesList::SetTitle(const std::string &title)
{
	this->title = title;
}



NCursesList::NCursesList() : NCursesListFrame()
{
	this->data.reserve(this->BUFFER_MAX_SIZE);
}
NCursesList::NCursesList(WINDOW *parent) : NCursesListFrame(parent) {}



NCursesList::~NCursesList() {}
