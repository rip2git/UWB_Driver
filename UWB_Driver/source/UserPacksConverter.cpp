#include "UserPacksConverter.h"


UserPackHL UserPacksConverter::ToHL(const UserPackFW &pack)
{
	UserPackHL _pack;

	_pack.FCmd = pack.FCmd;
	_pack.SCmd = pack.SCmd;
	_pack.TotalSize = pack.TotalSize;
	_pack.Data = pack.Data;

	return _pack;
}



UserPackHL UserPacksConverter::ToHL(const std::vector <UserPackFW> &pack)
{
	UserPackHL _pack;

	if (pack.size() > 0) {
		_pack.FCmd = pack[0].FCmd;
		_pack.SCmd = pack[0].SCmd;
		_pack.TotalSize = 0;
		_pack.Data.reserve(UserPackHL::DATA_MAX_SIZE);

		for (size_t i = 0; i < pack.size(); ++i) {
			_pack.Data.insert(_pack.Data.begin() + _pack.TotalSize, pack[i].Data.begin(), pack[i].Data.end());
			_pack.TotalSize += pack[i].Data.size();
		}
	}

	return _pack;
}



std::vector<UserPackFW> UserPacksConverter::ToFW(const UserPackHL &pack, const uint8_t buffer_size)
{
	std::vector<UserPackFW> _pack;
	uint8_t remainder = static_cast <uint8_t> (pack.TotalSize % buffer_size);

	if ( remainder )
		_pack.resize(pack.TotalSize / buffer_size + 1);
	else
		_pack.resize(pack.TotalSize / buffer_size);

	auto pdit = pack.Data.begin();
	for (auto it = _pack.begin(); it != _pack.end(); ++it) {
		it->FCmd = pack.FCmd;
		it->SCmd = pack.SCmd;
		if ( remainder && (it + 1) == _pack.end() ) {
			it->TotalSize = remainder;
			it->Data.assign(pdit, pdit + remainder);
			break;
		}
		it->TotalSize = buffer_size;
		it->Data.assign(pdit, pdit + buffer_size);
		pdit += buffer_size;
	}

	return _pack;
}


