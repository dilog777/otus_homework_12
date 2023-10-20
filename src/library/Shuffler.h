#pragma once

#include <string>
#include <vector>



class Shuffler
{
public:
	using StringList = std::vector<std::string>;
	using Data = std::vector<StringList>;

	Shuffler(const Data &data);

	Data shuffle(int redusersCount);

private:
	const Data &_sourceData;
	std::vector<int> _stringIndexes;

	bool readNextString(std::string &str);
};
