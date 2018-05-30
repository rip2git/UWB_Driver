
#include "NCurses.h"




void NCurses::RegisterObject(NCursesObject *object)
{
//	object->SetParent(stdscr);
	this->objects.push_back( object );
}



void NCurses::UnRegisterObject(NCursesObject *object)
{
	;
}



void NCurses::PrintDBG(std::string dbg)
{
	int row, col;
	getmaxyx(stdscr, row, col);
//	clear();
	mvwprintw(stdscr, row - 1, 0, dbg.c_str(), row, col);
	refresh();
}



void NCurses::Refresh()
{
//	clear();
	for (auto &o : objects)
		o->Refresh();
//	refresh();
}


NCurses::NCurses()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
}



NCurses::~NCurses()
{
	endwin();
}

