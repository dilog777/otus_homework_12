#include "Shuffler.h"



Shuffler::Shuffler(const Data &data)
	: _sourceData { data }
{
}



Shuffler::Data Shuffler::shuffle(int newSize)
{
	Data result(newSize);
	_currentString = std::vector<int>(_sourceData.size(), 0);

	int idx = -1;
	std::string lastStr;
	std::string nextStr;
	while (readNextString(nextStr))
	{
		if (lastStr != nextStr)
		{
			idx = (idx + 1) % newSize;
			lastStr = nextStr;
		}
		
		result[idx].push_back(nextStr);
	}

	return result;
}



bool Shuffler::readNextString(std::string &str)
{
	int vectorIdx = -1;
	std::string result;
	int sourceSize = _sourceData.size();
	for (int i = 0; i < sourceSize; ++i)
	{
		int stringIdx = _currentString[i];
		if (stringIdx < 0)
			continue;

		auto &sourceStr = _sourceData[i][stringIdx];
		if (result.empty() || sourceStr < result)
		{
			result = sourceStr;
			vectorIdx = i;
		}
	}
	
	if (vectorIdx == -1)
		return false;

	str = result;

	int &stringIdx = _currentString[vectorIdx];
	stringIdx++;

	size_t stringSize = _sourceData[vectorIdx].size();
	if (stringIdx >= static_cast<int>(stringSize))
		stringIdx = -1;

	return true;
}
