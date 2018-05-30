#ifndef NCursesTableFrame_H_
#define NCursesTableFrame_H_

#include "NCursesObject.h"
#include <vector>
#include <string>

/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
class NCursesTableFrame : public NCursesObject {
public:
	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	virtual int HandleCommand(long command) override {};

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 *
	 * NOTE: data[i][j], where i - column, j - row
	 *
	 * -----------------------------------------------------------------------------------------
	 * */
	template <typename TYPE>
	void SetData(
			const std::string &title,
			const std::vector <std::vector <TYPE> > &data,
			const std::vector <std::string> &columnsNames,
			std::string (*to_string)(TYPE) = std::to_string);

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	NCursesTableFrame();
	NCursesTableFrame(WINDOW *parent);
	virtual ~NCursesTableFrame();

private:
	using NCursesObject::Resize;
	void _SetData(
			const std::string &title,
			const std::vector <std::vector <std::string> > &_data,
			const std::vector <std::string> &columnsNames );
};



#if 1 /* *************** IMPLEMENTATION ************************** */



template <typename TYPE>
void NCursesTableFrame::SetData(
		const std::string &title,
		const std::vector <std::vector <TYPE> > &data,
		const std::vector <std::string> &columnsNames,
		std::string (*to_string)(TYPE))
{
	std::vector <std::vector <std::string> > _data;

	_data.resize(data.size());
	for (size_t i = 0; i < data.size(); ++i) {
		_data[i].resize(data[i].size());
		for (size_t j = 0; j < data[i].size(); ++j) {
			_data[i][j] = to_string(data[i][j]);
		}
	}

	this->_SetData(title, _data, columnsNames);
}



#endif /* IMPLEMENTATION */

#endif /* NCURESESTABLE_H_ */
