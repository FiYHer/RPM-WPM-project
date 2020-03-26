#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Config
{
private:
	char config_location_c_[255];
	std::string config_location;

public:
	bool initialize(std::string exe_path);

	void read_integer(std::string section, std::string key, int& value) const;
	void read_boolean(std::string section, std::string key, bool& value) const;
	void write_integer(std::string section, std::string key, int value) const;
	void write_boolean(std::string section, std::string key, bool value) const;

	void load_config() const;
	void save_config() const;

};

extern Config config;