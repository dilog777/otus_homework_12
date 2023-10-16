#include <iostream>

#include <filesystem>

#include <boost/format.hpp>

#include <FileSplitter.h>
#include <StringConverter.hpp>

const char *const USAGE_MESSAGE = "Usage: mapreduce <src> <mnum> <rnum>";
const char *const FILE_NOT_EXIST_MESSAGE = "File '%s' not exist";
const char *const RANGE_ERROR_MESSAGE = "The <%s> value must be greater than zero";



int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::cout << USAGE_MESSAGE << std::endl;
		return EXIT_FAILURE;
	}

	std::string srcPath = argv[1];
	bool exist = std::filesystem::exists(srcPath);
	if (!exist)
	{
		std::cout << USAGE_MESSAGE << std::endl;
		std::cout << (boost::format { FILE_NOT_EXIST_MESSAGE } % srcPath) << std::endl;
		return EXIT_FAILURE;
	}

	auto mnum = StringConverter::stringToType<int>(argv[2]);
	if (mnum <= 0)
	{
		std::cout << USAGE_MESSAGE << std::endl;
		std::cout << (boost::format { RANGE_ERROR_MESSAGE } % "mnum") << std::endl;
		return EXIT_FAILURE;
	}

	auto rnum = StringConverter::stringToType<int>(argv[3]);
	if (rnum <= 0)
	{
		std::cout << USAGE_MESSAGE << std::endl;
		std::cout << (boost::format { RANGE_ERROR_MESSAGE } % "rnum") << std::endl;
		return EXIT_FAILURE;
	}

	auto parts = FileSplitter::split(srcPath, 3);



	return EXIT_SUCCESS;
}
