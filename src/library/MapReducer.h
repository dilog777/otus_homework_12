#pragma once

#include <functional>
#include <string>
#include <vector>



class MapReducer
{
public:
	MapReducer(const std::string &srcPath, int mappersCount, int redusersCount);

	using MapFunctor = std::function<std::string(const std::string &)>;
	void setMapFunctor(const MapFunctor &mapFunctor);

	using ReduceFunctor = std::function<std::vector<std::string>(const std::vector<std::string> &)>;
	void setReduceFunctor(const ReduceFunctor &reduceFunctor);

	std::vector<std::string> exec();

private:
	std::string _srcPath;
	int _mappersCount;
	int _redusersCount { 0 };

	MapFunctor _mapFunctor;
	ReduceFunctor _reduceFunctor;

	std::vector<std::string> map(std::uintmax_t startPos, std::uintmax_t endPos) const;
	std::vector<std::string> reduce(const std::vector<std::string> &strings) const;
};
