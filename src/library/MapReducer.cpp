#include "MapReducer.h"

#include <fstream>
#include <future>

#include <FileSplitter.h>



MapReducer::MapReducer(const std::string &srcPath, int mappersCount, int redusersCount)
	: _srcPath { srcPath }
	, _mappersCount { mappersCount }
	, _redusersCount { redusersCount }
{
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
	auto parts = FileSplitter::split(_srcPath, _mappersCount);

	std::vector<std::future<std::vector<std::string>>> futures;
	futures.reserve(parts.size());
	for (const auto &part : parts)
	{
		futures.push_back(std::async(std::launch::async, &MapReducer::map, this, part.first, part.second));
	}

	std::vector<std::string> mapResult;
	for (auto &future : futures)
	{
		auto vector = future.get();
		mapResult.insert(mapResult.end(), vector.begin(), vector.end());
	}

	std::sort(mapResult.begin(), mapResult.end());

	auto reduceResult = reduce(mapResult);

	return reduceResult;
}



std::vector<std::string> MapReducer::map(std::uintmax_t startPos, std::uintmax_t endPos) const
{
	std::ifstream file(_srcPath);
	if (!file.is_open())
		return {};

	std::vector<std::string> result;

	file.seekg(startPos);
	while (file.tellg() < endPos)
	{
		std::string line;
		std::getline(file, line);

		result.push_back(_mapFunctor(line));
	}

	file.close();

	std::sort(result.begin(), result.end());

	return result;
}



std::vector<std::string> MapReducer::reduce(const std::vector<std::string> &strings) const
{
	auto result = _reduceFunctor(strings);
	return result;
}
