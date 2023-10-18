#pragma once

#include <vector>



namespace DuplicateSearcer
{
	template <class T>
	std::vector<T> duplicates(const std::vector<T> &source)
	{
		std::vector<std::string> duplicates;
		auto it = std::adjacent_find(source.begin(), source.end());

		while (it != source.end())
		{
			duplicates.push_back(*it);
			it = std::upper_bound(it, source.end(), *it);
			it = std::adjacent_find(it, source.end());
		}

		return duplicates;
	}
}
