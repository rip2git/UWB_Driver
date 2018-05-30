#ifndef NCursesTable_H_
#define NCursesTable_H_

#include <NCursesTableFrame.h>
#include <memory>
#include "std_string.h"

/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
class NCursesTable : public NCursesTableFrame {
public:
	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 *
	 * NOTE: sets and refreshes
	 *
	 * -----------------------------------------------------------------------------------------
	 * */
	void SetData(
			const std::vector <std::vector <std::string> > &data,
			const std::vector <std::string> &columnsNames );

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	void SetTitle(const std::string &title);

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	void Refresh() override;

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	void SetValue(const std::string &value, size_t column, size_t row);

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	void SetColumnName(const std::string &name, size_t column);

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	NCursesTable();
	NCursesTable(WINDOW *parent);
	virtual ~NCursesTable();

private:
	std::string title;
	std::vector <std::vector <std::string> > data;
	std::vector <std::string> columnsNames;
};



#if 0 /* *************** IMPLEMENTATION ************************** */



template <typename TYPE>
void NCursesTable<TYPE>::SetData(
			const std::vector <std::vector <TYPE> > &data,
			const std::vector <std::string> &columnsNames )
{
	this->data = data;
	this->columnsNames = columnsNames;
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames);
}



template <>
void NCursesTable<std::string>::SetData(
			const std::vector <std::vector <std::string> > &data,
			const std::vector <std::string> &columnsNames )
{
	this->data = data;
	this->columnsNames = columnsNames;
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames, str_to_string);
}




template <typename TYPE>
void NCursesTable<TYPE>::SetTitle(const std::string &title)
{
	this->title = title;
}



template <typename TYPE>
void NCursesTable<TYPE>::Refresh()
{
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames);
	NCursesTableFrame::Refresh();
}



template <>
void NCursesTable<std::string>::Refresh()
{
	NCursesTableFrame::SetData(this->title, this->data, this->columnsNames, str_to_string);
	NCursesTableFrame::Refresh();
}



template <typename TYPE>
void NCursesTable<TYPE>::SetValue(TYPE value, size_t column, size_t row)
{
	if (this->data.size() < column+1)
		this->data.resize(column+1);
	if (this->data[column].size() < row+1)
		this->data[column].resize(row+1);
	this->data[column][row] = value;
}



template <typename TYPE>
void NCursesTable<TYPE>::SetColumnName(std::string name, size_t column)
{
	if (this->columnsNames.size() < column+1)
		this->columnsNames.resize(column+1);
	this->columnsNames[column] = name;
}



template <typename TYPE>
NCursesTable<TYPE>::NCursesTable(WINDOW *parent) : NCursesTableFrame(parent) {}



template <typename TYPE>
NCursesTable<TYPE>::~NCursesTable() {}



#endif /* IMPLEMENTATION */

#endif /* NCursesTable_H_ */
