
#ifndef NCursesListFrameVIEW_H_
#define NCursesListFrameVIEW_H_

#include "NCursesObject.h"
#include <vector>
#include <string>

/*! ----------------------------------------------------------------------------------------
 * @brief:
 * -----------------------------------------------------------------------------------------
 * */
class NCursesListFrame : public NCursesObject {
public:
	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	bool isTitled;

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	virtual int HandleCommand(long command) override {};

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	template <typename TYPE>
	void SetData(
			const std::string &title,
			const std::vector <TYPE> &data,
			std::string (*to_string)(TYPE) = std::to_string);

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	void Resize(size_t charsPerLine, size_t lines) override;

	/*! ----------------------------------------------------------------------------------------
	 * @brief:
	 * -----------------------------------------------------------------------------------------
	 * */
	NCursesListFrame();
	NCursesListFrame(WINDOW *parent);
	virtual ~NCursesListFrame();

protected:
	size_t lines;

private:
	void _SetData(
			const std::string &title,
			const std::vector <std::string> &_data);
};



#if 1 /* *************** IMPLEMENTATION ************************** */



template <typename TYPE>
void NCursesListFrame::SetData(
		const std::string &title,
		const std::vector <TYPE> &data,
		std::string (*to_string)(TYPE))
{
	std::vector <std::string> _data;

	_data.resize(data.size());
	for (size_t i = 0; i < data.size(); ++i) {
		_data[i] = to_string(data[i]);
	}

	this->_SetData(title, _data);
}



#endif /* IMPLEMENTATION */

#endif /* NCursesListFrameVIEW_H_ */
