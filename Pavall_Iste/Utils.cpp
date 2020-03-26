#include "Utils.hpp"

namespace utils
{
	std::string remove_exe_name(std::string exe_path)
	{
		const auto pos = exe_path.find_last_of('\\');
		if(pos != std::string::npos)
		{
			exe_path = exe_path.substr(0, pos + 1);
			return exe_path;
		}

		return "";
	}
}