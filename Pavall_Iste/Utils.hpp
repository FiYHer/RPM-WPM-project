#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <iostream>

namespace utils
{
	std::string remove_exe_name(std::string exe_path);
}