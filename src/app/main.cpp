#include <iostream>

#include <filesystem>

#include <boost/format.hpp>

#include <DuplicateSearcer.hpp>
#include <MapReducer.h>
#include <StringConverter.hpp>

const char *const USAGE_MESSAGE = "Usage: mapreduce <src> <mnum> <rnum>";
const char *const FILE_NOT_EXIST_MESSAGE = "File '%s' not exist";
const char *const RANGE_ERROR_MESSAGE = "The <%s> value must be greater than zero";
const char *const RESULT_SUCCESS_MESSAGE = "Minimum length %d characters";
const char *const RESULT_FAILURE_MESSAGE = "Minimum length not found";



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

	int length = 1;

	MapReducer mapReducer(srcPath, mnum, rnum);
	mapReducer.setMapFunctor([&length](const std::string &str) { return str.substr(0, length); });
	mapReducer.setReduceFunctor([](const std::vector<std::string> &strings) { return DuplicateSearcer::duplicates(strings); });

	auto stringShorterLength = [&length](const std::string &str){ return (str.length() < static_cast<size_t>(length)); };

	while (length < INT_MAX)
	{
		auto duplicates = mapReducer.exec();
		if (duplicates.empty())
		{
			std::cout << (boost::format { RESULT_SUCCESS_MESSAGE } % length) << std::endl;
			break;
		}

		bool allStringsShorterLength = std::all_of(duplicates.begin(), duplicates.end(), stringShorterLength);
		if (allStringsShorterLength)
		{
			std::cout << RESULT_FAILURE_MESSAGE << std::endl;
			break;
		}

		++length;
	}

	return EXIT_SUCCESS;
}
