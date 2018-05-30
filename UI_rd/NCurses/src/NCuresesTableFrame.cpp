
#include <NCursesTableFrame.h>
#include <algorithm>



void NCursesTableFrame::_SetData(
		const std::string &title,
		const std::vector <std::vector <std::string> > &_data,
		const std::vector <std::string> &columnsNames )
{
	std::vector <int> widths;
	size_t _max_height = 0;

	{ // defines data in string format and max columns widths
		std::vector <std::vector <int> > _sizes;
		_sizes.resize(_data.size());

		for (size_t i = 0; i < _data.size(); ++i) {
			_sizes[i].resize(_data[i].size());
			for (size_t j = 0; j < _data[i].size(); ++j) {
				_sizes[i][j] = _data[i][j].size();
			}
			if (_max_height < _data[i].size())
				_max_height = _data[i].size();
			_sizes[i].push_back(columnsNames[i].size());
		}

		widths.reserve(_sizes.size());
		for (auto &s : _sizes)
			widths.push_back( s[ std::distance(s.begin(), std::max_element(s.begin(), s.end())) ] );
	}

	if (widths.size() == 0) return;

	{ // defines width and height
		//size_t prev_w = this->width, prev_h = this->height;
		this->width = widths.size() + 1;
		for (auto &w : widths)
			this->width += w;

		this->height = _max_height + 6;

		this->Resize(this->width, this->height);
	}

	{ // borders
//		wclear(this->win);
		box(this->win, 0, 0);

		// title lower line
		mvwaddch(this->win, 2, 0, ACS_LTEE);
		mvwhline(this->win, 2, 1, ACS_HLINE, this->width - 2);
		mvwaddch(this->win, 2, this->width - 1, ACS_RTEE);

		// names lower line
		mvwaddch(this->win, 4, 0, ACS_LTEE);
		mvwhline(this->win, 4, 1, ACS_HLINE, this->width - 2);
		mvwaddch(this->win, 4, this->width - 1, ACS_RTEE);

		// names vertical lines
		for (size_t i = 0, vl = 0; i < widths.size()-1; ++i) {
			vl += widths[i] + 1;
			mvwvline(this->win, 2, vl, ACS_VLINE, this->height - 3);

			// cross-points
			mvwaddch(this->win, 2, vl, ACS_TTEE);
			mvwaddch(this->win, 4, vl, ACS_PLUS);
			mvwaddch(this->win, this->height - 1, vl, ACS_BTEE);
		}
	}

	// title
	if (title.size() <= this->width - 2)
		mvwprintw(this->win, 1, (this->width - 2) / 2 - title.size() / 2 + 1, "%s", title.c_str());

	// names
	for (size_t i = 0, vl = 1; i < columnsNames.size(); ++i) {
		mvwprintw(this->win, 3, widths[i] / 2 - columnsNames[i].size() / 2 + vl, "%s", columnsNames[i].c_str());
		vl += widths[i] + 1;
	}

	// data
	for (size_t i = 0, vl = 1; i < _data.size(); ++i) {
		for (size_t j = 0, hl = 5; j < _data[i].size(); ++j) {
			mvwprintw(this->win, j + hl, widths[i] / 2 - _data[i][j].size() / 2 + vl, "%s", _data[i][j].c_str());
		}
		vl += widths[i] + 1;
	}
}



NCursesTableFrame::NCursesTableFrame() : NCursesObject() {}
NCursesTableFrame::NCursesTableFrame(WINDOW *parent) : NCursesObject(parent) {}



NCursesTableFrame::~NCursesTableFrame() {}

