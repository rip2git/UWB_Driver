
#include "NCursesTable.h"



void NCursesTable::SetData(
			const std::vector <std::vector <std::string> > &data,
			const std::vector <std::string> &columnsNames )
{
	this->data = data;
	this->columnsNames = columnsNames;
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames, str_to_string);
}




void NCursesTable::SetTitle(const std::string &title)
{
	this->title = title;
}



void NCursesTable::Refresh()
{
	NCursesTableFrame::Refresh();
}



void NCursesTable::SetValue(const std::string &value, size_t column, size_t row)
{
	if (this->data.size() < column+1)
		this->data.resize(column+1);
	if (this->data[column].size() < row+1)
		this->data[column].resize(row+1);
	this->data[column][row] = value;
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames, str_to_string);
}



void NCursesTable::SetColumnName(const std::string &name, size_t column)
{
	if (this->columnsNames.size() < column+1)
		this->columnsNames.resize(column+1);
	this->columnsNames[column] = name;
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames, str_to_string);
}



NCursesTable::NCursesTable() : NCursesTableFrame() {}
NCursesTable::NCursesTable(WINDOW *parent) : NCursesTableFrame(parent) {}



NCursesTable::~NCursesTable() {}
