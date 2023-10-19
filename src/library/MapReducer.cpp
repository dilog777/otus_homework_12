#include "MapReducer.h"

#include <filesystem>
#include <fstream>
#include <future>

#include "FileSplitter.h"
#include "Shuffler.h"


MapReducer::MapReducer(const std::string &srcPath, int mappersCount, int redusersCount)
	: _srcPath { srcPath }
	, _mappersCount { mappersCount }
	, _redusersCount { redusersCount }
{
}



void MapReducer::setSaveReduceResult(bool saveReduceResult)
{
	_saveReduceResult = saveReduceResult;
}



void MapReducer::setMapFunctor(const MapFunctor &mapFunctor)
{
	_mapFunctor = mapFunctor;
}



void MapReducer::setReduceFunctor(const ReduceFunctor &reduceFunctor)
{
	_reduceFunctor = reduceFunctor;
}



std::vector<std::string> MapReducer::exec()
{
	auto fileParts = FileSplitter::split(_srcPath, _mappersCount);

	std::vector<std::future<std::vector<std::string>>> futures;
	for (const auto &filePart : fileParts)
		futures.push_back(std::async(std::launch::async, &MapReducer::map, this, filePart.first, filePart.second));

	Shuffler::Data mapResult;
	for (auto &future : futures)
		mapResult.push_back(future.get());

	Shuffler shufler(mapResult);
	auto shuffleResult = shufler.shuffle(_redusersCount);

	int rnum = 0;
	futures.clear();
	for (const auto &strings : shuffleResult)
		futures.push_back(std::async(std::launch::async, &MapReducer::reduce, this, strings, ++rnum));

	std::vector<std::string> reduceResult;
	for (auto &future : futures)
	{
		auto vector = future.get();
		reduceResult.insert(reduceResult.end(), vector.begin(), vector.end());
	}

	return reduceResult;
}



std::vector<std::string> MapReducer::map(uint64_t startPos, uint64_t endPos) const
{
	std::ifstream file(_srcPath);
	if (!file.is_open())
		return {};

	std::vector<std::string> result;

	file.seekg(startPos);
	while (static_cast<uint64_t>(file.tellg()) < endPos)
	{
		std::string line;
		std::getline(file, line);
		result.push_back(_mapFunctor(line));
	}

	file.close();

	std::sort(result.begin(), result.end());

	return result;
}



std::vector<std::string> MapReducer::reduce(const std::vector<std::string> &strings, int rnum) const
{
	auto result = _reduceFunctor(strings);

	if (_saveReduceResult)
	{
		std::filesystem::path filePath { _srcPath };
		std::string newName = filePath.stem().string() + "_" + std::to_string(rnum) + filePath.extension().string();
		filePath.replace_filename(newName);

		std::ofstream out;
		out.open(filePath);
		if (out.is_open())
		{
			for (const auto &str : result)
				out << str << std::endl;
		}
		out.close();
	}

	return result;
}
