
#include "NCursesObject.h"



void NCursesObject::Refresh()
{
//	wclear(this->win);
	wrefresh(this->win);
}



void NCursesObject::Resize(size_t width, size_t height)
{
	this->width = width;
	this->height = height;
	delwin(this->win);
	this->win = newwin(this->height, this->width, this->y, this->x);
}



void NCursesObject::SetParent(WINDOW *parent)
{
	this->win->_parent = parent;
}



int NCursesObject::GetWidth() const
{
	return this->width;
}
int NCursesObject::GetHeight() const
{
	return this->height;
}



void NCursesObject::SetPosition(int y, int x)
{
	this->x = x;
	this->y = y;
	mvwin(this->win, y, x);
}



void NCursesObject::Move(int y, int x)
{
	this->x += x;
	this->y += y;
	mvwin(this->win, this->y, this->x);
}



int NCursesObject::GetPositionX() const
{
	return this->x;
}
int NCursesObject::GetPositionY() const
{
	return this->y;
}



NCursesObject::NCursesObject(WINDOW *parent)
{
	this->isSelected = false;
	this->width = 1;
	this->height = 1;
	this->x = 1;
	this->y = 1;
	this->win = newwin(this->height, this->width, this->y, this->x);
	this->win->_parent = parent;
}



NCursesObject::NCursesObject()
{
	this->isSelected = false;
	this->width = 1;
	this->height = 1;
	this->x = 1;
	this->y = 1;
	this->win = newwin(this->height, this->width, this->y, this->x);
}



NCursesObject::~NCursesObject()
{
	delwin(this->win);
}
