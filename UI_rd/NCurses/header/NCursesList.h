#ifndef NCursesList_H_
#define NCursesList_H_

#include <NCursesListFrame.h>
#include <memory>
#include "std_string.h"

class NCursesList : public NCursesListFrame {
public:
	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 *
	 * NOTE: sets and refreshes
	 *
	 * -----------------------------------------------------------------------------------------
	 * */
	void SetData(const std::vector <std::string> &data);

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	void PushBack(const std::string &str);

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
	NCursesList();
	NCursesList(WINDOW *parent);
	virtual ~NCursesList();

private:
	size_t BUFFER_MAX_SIZE = 20;
	std::string title;
	std::vector <std::string> data;
};



#if 0 /* *************** IMPLEMENTATION ************************** */



template <typename TYPE>
void NCursesList<TYPE>::SetData(const std::vector <TYPE> &data)
{
	this->data = data;
	NCursesListFrame::SetData(this->title, this->data);
}



template <>
void NCursesList<std::string>::SetData(const std::vector <std::string> &data)
{
	this->data = data;
	NCursesListFrame::SetData(this->title, this->data, str_to_string);
}



template <typename TYPE>
void NCursesList<TYPE>::Refresh()
{
	NCursesListFrame::SetData(this->title, this->data);
	NCursesListFrame::Refresh();
}



template <>
void NCursesList<std::string>::Refresh()
{
	NCursesListFrame::SetData(this->title, this->data, str_to_string);
	NCursesListFrame::Refresh();
}



template <typename TYPE>
void NCursesList<TYPE>::SetTitle(const std::string &title)
{
	this->title = title;
}



template <typename TYPE>
NCursesList<TYPE>::NCursesList(WINDOW *parent) : NCursesListFrame(parent) {}



template <typename TYPE>
NCursesList<TYPE>::~NCursesList() {}



#endif /* IMPLEMENTATION */

#endif /* NCursesList_H_ */

