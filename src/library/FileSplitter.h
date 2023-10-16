#pragma once

#include <string>
#include <vector>



class FileSplitter
{
public:
	using FilePart = std::pair<std::uintmax_t, std::uintmax_t>;

	static std::vector<FilePart> split(const std::string &filePath, int parts);
};
