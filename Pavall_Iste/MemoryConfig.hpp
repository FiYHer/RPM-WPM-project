#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <sstream>

class MemoryConfig
{
private:
	std::string config_location;

public:
	bool initialize(std::string exe_path, bool fetch_from_web);

	uintptr_t get_memory_var(std::string var, bool debug_print = true) const;

	void get_memory_vars() const;
	void check_if_exists() const;
	
	uintptr_t get_memory_var_web(std::string var, bool debug_print) const;
	void get_from_web() const;
};

extern MemoryConfig memory_config;
