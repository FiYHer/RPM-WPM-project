#include "MemoryConfig.hpp"

#include "Utils.hpp"
#include "Addresses.hpp"
#include "Include.hpp"

MemoryConfig memory_config;

bool MemoryConfig::initialize(std::string exe_path, bool fetch_from_web)
{
	//Same as before, get exe path for folder to get config location
	std::string new_path = utils::remove_exe_name(exe_path);

	if (new_path.length() <= 0) return false;

	config_location = new_path + "memory.cfg";
	get_from_web();

	return true;
}

uintptr_t MemoryConfig::get_memory_var(std::string var, const bool debug_print) const
{
	//Gets the stream to the file to read from it
	std::ifstream file_read(config_location);

	if (!file_read)
		return NULL;

	std::string file_line;
	//Loop through the lines of the file
	while (std::getline(file_read, file_line))
	{
		//If it finds the variable its looking for
		if (file_line.find(var) != std::string::npos)
		{
			//Check its valid length
			if (var.length() > 4)
			{
				//Gets position of the offset itself
				const auto pos = var.length() + 1;
				const auto addr = file_line.substr(pos, var.length());

				//Converts it from string to hex
				uintptr_t return_addr = NULL;
				std::stringstream convert_stream;
				convert_stream << addr;
				convert_stream >> std::hex >> return_addr;

				//If debug print is on, print that it found it
				if (debug_print) {
					std::cout << "Found " << var << " at 0x" << std::hex << std::uppercase << return_addr << std::endl;
				}

				//Return found address/offset
				return return_addr;
			}
		}
	}

	//Could not find it for some reason
	if (debug_print) {
		std::cout << "Could not find memory var " << var << std::endl;
	}

	return NULL;
}

void MemoryConfig::get_memory_vars() const
{
	addresses::local_player = get_memory_var("localplayer");
	addresses::entity_list = get_memory_var("entitylist");
	addresses::force_attack = get_memory_var("forceattack");
	addresses::force_jump = get_memory_var("forcejump");
	addresses::viewmatrix = get_memory_var("viewmatrix");
	addresses::client_state = get_memory_var("clientstate");
	addresses::glow_object = get_memory_var("glow_object");

	offsets::aim_punch = get_memory_var("aimpunch");
	offsets::flags = get_memory_var("flags");
	offsets::health = get_memory_var("health");
	offsets::team_num = get_memory_var("teamnum");
	offsets::life_state = get_memory_var("lifestate");
	offsets::origin = get_memory_var("origin");
	offsets::viewoffset = get_memory_var("viewoffset");
	offsets::bone_matrix = get_memory_var("bonematrix");
	offsets::crosshairid = get_memory_var("crosshairid");
	offsets::dormant = get_memory_var("dormant");
	offsets::viewangles = get_memory_var("viewangles");
	offsets::glow_index = get_memory_var("glow_index");
	offsets::active_weapon = get_memory_var("active_weapon");
	offsets::item_index = get_memory_var("item_index");
	offsets::current_map_dir = get_memory_var("current_map_dir");
}

void MemoryConfig::check_if_exists() const
{
	//Gets stream to read from it
	std::ifstream file_read(config_location);
	//If it can't find the file, make one and just put some values in (outdated ones)
	if (!file_read)
	{
		file_read.close();

		std::ofstream file_write(config_location);
		file_write << "localplayer=0xAA00FC" << std::endl;
		file_write << "entitylist=0x4A7D0F4" << std::endl;
		file_write << "forceattack=0x2EBF474" << std::endl;
		file_write << "forcejump=0x4F1437C" << std::endl;
		file_write << "clientstate=0x57D894" << std::endl;
		file_write << "viewmatrix=0x4A6EB34" << std::endl;
		file_write << "glow_object=0x4FB2540" << std::endl;
		file_write << "game_directory=0x626C70" << std::endl;
		file_write << "aimpunch=0x301C" << std::endl;
		file_write << "flags=0x100" << std::endl;
		file_write << "health=0xFC" << std::endl;
		file_write << "teamnum=0xF0" << std::endl;
		file_write << "lifestate=0x25B" << std::endl;
		file_write << "origin=0x134" << std::endl;
		file_write << "viewoffset=0x104" << std::endl;
		file_write << "bonematrix=0x2698" << std::endl;
		file_write << "crosshairid=0xB2A4" << std::endl;
		file_write << "dormant=0xE9" << std::endl;
		file_write << "viewangles=0x4D10" << std::endl;
		file_write << "active_weapon=0x2EE8" << std::endl;
		file_write << "item_index=0x2F88" << std::endl;
		file_write << "glow_index=0xA310" << std::endl;
		file_write << "current_map_dir=0x28C";
		file_write.close();
	}
}

uintptr_t MemoryConfig::get_memory_var_web(std::string var, bool debug_print) const
{
	//Open up an internet connection
	const auto internet = InternetOpen("browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);

	//Check if its valid
	if (!internet) return NULL;

	//Open the URL that holds the updated offsets
	const char* target = "https://github.com/MrLotsmon/csoffsets/blob/master/csgo.cs";
	const auto internet_address = InternetOpenUrl(internet, target, nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);

	//If it's invalid for some reason, close connection
	if (!internet_address)
	{
		InternetCloseHandle(internet);
		return NULL;
	}

	//Buffers
	char* data = nullptr;
	unsigned long bytes_read = NULL;
	unsigned long data_size = NULL;

	//Read the website 2000 bytes per loop until its fully read,  this will be put in the char* data buffer
	do
	{
		char buffer[2000];
		InternetReadFile(internet_address, reinterpret_cast<LPVOID>(buffer), _countof(buffer), &bytes_read);
		auto* temp_data = new char[data_size + bytes_read];
		memcpy(temp_data, data, data_size);
		memcpy(temp_data + data_size, buffer, bytes_read);
		delete[] data;
		data = temp_data;
		data_size += bytes_read;
	} while (bytes_read);

	//Convert char* to std::string
	auto data_str = std::string(data);
	//Finds position of an offset thats passed in the var variable
	auto pos_start = data_str.find(var);
	pos_start = data_str.find("0x", pos_start + 1);

	//If it found one
	if (pos_start != std::string::npos)
	{
		//Get the end of the offset
		const auto pos_end = data_str.find_first_of(';', pos_start);
		//If there is an end
		if (pos_end != std::string::npos)
		{
			//Get the address in string format
			const auto addr = data_str.substr(pos_start, pos_end - pos_start);

			//Convert from string to hex
			uintptr_t return_addr = NULL;
			std::stringstream convert_stream;
			convert_stream << addr;
			convert_stream >> std::hex >> return_addr;

			//If debug print, print it found it
			if (debug_print) {
				std::cout << "Found " << var << " at 0x" << std::hex << std::uppercase << return_addr << std::endl;
			}

			//Return it
			return return_addr;
		}
	}

	//Could not find it for some reason, close program
	if (debug_print) {
		std::cout << "Could not find memory var " << var << std::endl;
		InternetCloseHandle(internet_address);
		InternetCloseHandle(internet);
		exit(0);
	}

	InternetCloseHandle(internet_address);
	InternetCloseHandle(internet);

	return NULL;
}

void MemoryConfig::get_from_web() const
{
	addresses::local_player = get_memory_var_web("dwLocalPlayer", true);
	addresses::entity_list = get_memory_var_web("dwEntityList", true);
	addresses::force_attack = get_memory_var_web("dwForceAttack", true);
	addresses::force_jump = get_memory_var_web("dwForceJump", true);
	addresses::viewmatrix = get_memory_var_web("dwViewMatrix", true);
	addresses::client_state = get_memory_var_web("dwClientState", true);
	addresses::glow_object = get_memory_var_web("dwGlowObjectManager", true);
	addresses::game_dir = get_memory_var_web("dwGameDir", true);

	offsets::aim_punch = get_memory_var_web("m_aimPunchAngle", true);
	offsets::flags = get_memory_var_web("m_fFlags", true);
	offsets::health = get_memory_var_web("m_iHealth", true);
	offsets::team_num = get_memory_var_web("m_iTeamNum", true);
	offsets::life_state = get_memory_var_web("m_lifeState", true);
	offsets::origin = get_memory_var_web("m_vecOrigin", true);
	offsets::viewoffset = get_memory_var_web("m_vecViewOffset", true);
	offsets::bone_matrix = get_memory_var_web("m_dwBoneMatrix", true);
	offsets::crosshairid = get_memory_var_web("m_iCrosshairId", true);
	offsets::dormant = 0xED; // if cheat works weird check this
	offsets::viewangles = get_memory_var_web("dwClientState_ViewAngles", true);
	offsets::glow_index = get_memory_var_web("m_iGlowIndex", true);
	offsets::active_weapon = get_memory_var_web("m_hActiveWeapon", true);
	offsets::item_index = get_memory_var_web("m_iItemDefinitionIndex", true);
	offsets::current_map_dir = get_memory_var_web("dwClientState_MapDirectory", true);
}