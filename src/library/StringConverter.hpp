#pragma once

#include <string>

#include <boost/lexical_cast.hpp>


namespace StringConverter
{
	template <class T>
	T stringToType(const std::string &string)
	{
		try
		{
			return boost::lexical_cast<T>(string);
		}
		catch (boost::bad_lexical_cast &)
		{
			return {};
		}
	}
}
