
#include <NCursesListFrame.h>



void NCursesListFrame::_SetData(
		const std::string &title,
		const std::vector <std::string> &_data)
{
	{ // borders
//		wclear(this->win);
		box(this->win, 0, 0);
	}

	size_t hl = 0;
	if (this->isTitled) {
		// title lower line
		mvwaddch(this->win, 2, 0, ACS_LTEE);
		mvwhline(this->win, 2, 1, ACS_HLINE, this->width - 2);
		mvwaddch(this->win, 2, this->width - 1, ACS_RTEE);
		// title
		if (title.size() <= this->width - 2)
			mvwprintw(this->win, 1, (this->width - 2) / 2 - title.size() / 2, "%s", title.c_str());
		hl = 3;
	} else {
		hl = 1;
	}

	// data
	for (size_t i = 0; i < _data.size(); ++i) {
		mvwprintw(this->win, hl+i, 1, "%s", _data[i].c_str());
	}
}



void NCursesListFrame::Resize(size_t charsPerLine, size_t lines)
{
	this->lines = lines;
	this->width = charsPerLine + 2;
	this->height = this->isTitled? lines + 4 : lines + 2;
	delwin(this->win);
	this->win = newwin(this->height, this->width, this->y, this->x);
}



NCursesListFrame::NCursesListFrame() : NCursesObject() {
	this->isTitled = false;
	this->lines = 1;
}
NCursesListFrame::NCursesListFrame(WINDOW *parent) : NCursesObject(parent) {
	this->isTitled = false;
	this->lines = 1;
}



NCursesListFrame::~NCursesListFrame() {}

