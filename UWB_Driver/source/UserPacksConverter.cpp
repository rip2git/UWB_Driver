#include "UserPacksConverter.h"


UserPackHL UserPacksConverter::ToHL(const UserPackFW &pack)
{
	UserPackHL _pack;

	_pack.Command = pack.Command;
	_pack.DestinationID = pack.DestinationID;
	_pack.TotalSize = pack.TotalSize;
	_pack.Data = pack.Data;

	return _pack;
}



std::vector<UserPackFW> UserPacksConverter::ToFW(const UserPackHL &pack)
{
	std::vector<UserPackFW> _pack;
	uint8_t remainder = static_cast <uint8_t> (pack.TotalSize % UserPackFW::MAX_DATA_SIZE);

	if ( remainder )
		_pack.resize(pack.TotalSize / UserPackFW::MAX_DATA_SIZE + 1);
	else
		_pack.resize(pack.TotalSize / UserPackFW::MAX_DATA_SIZE);

	auto pdit = pack.Data.begin();
	for (auto it = _pack.begin(); it != _pack.end(); ++it) {
		it->Command = pack.Command;
		it->DestinationID = pack.DestinationID;
		if ( remainder && (it + 1) == _pack.end() ) {
			it->TotalSize = remainder;
			it->Data.assign(pdit, pdit + remainder);
			break;
		}
		it->TotalSize = UserPackFW::MAX_DATA_SIZE;
		it->Data.assign(pdit, pdit + UserPackFW::MAX_DATA_SIZE);
		pdit += UserPackFW::MAX_DATA_SIZE;
	}

	return _pack;
}


