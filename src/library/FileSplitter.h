#pragma once

#include <string>
#include <vector>



class FileSplitter
{
public:
	using FilePart = std::pair<uint64_t, uint64_t>;

	static std::vector<FilePart> split(const std::string &filePath, int parts);
};
