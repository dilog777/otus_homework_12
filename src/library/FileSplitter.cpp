#include "FileSplitter.h"

#include <cassert>
#include <filesystem>
#include <fstream>



std::vector<FileSplitter::FilePart> FileSplitter::split(const std::string &filePath, int parts)
{
	assert(parts >= 0);
	
	std::error_code ec;
	std::uintmax_t fileSize = std::filesystem::file_size(filePath, ec);
	if (ec)
		return {};

	std::ifstream file(filePath);
	if (!file.is_open())
		return {};

	std::vector<FilePart> result;
	result.reserve(parts);

	std::uintmax_t startPos = 0;
	std::uintmax_t blockSize = fileSize / parts;
	for (int i = 1; i < parts; ++i)
	{
		std::uintmax_t endPos = blockSize * i;
		if (endPos <= startPos)
			continue;

		file.seekg(endPos);
		std::string line;
		std::getline(file, line);

		endPos = file.tellg();
		result.push_back({ startPos, endPos });
		startPos = endPos;
	}

	result.push_back({ startPos, fileSize });

	file.close();

	return result;
}
