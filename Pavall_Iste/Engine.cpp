#include "Engine.hpp"

#include "Include.hpp"

namespace engine
{
	uintptr_t get_client_state()
	{
		return memory.read_memory<uintptr_t>(memory.get_engine().module_base + addresses::client_state);
	}

	void force_jump()
	{
		memory.write_memory<bool>(memory.get_client().module_base + addresses::force_jump, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		memory.write_memory<bool>(memory.get_client().module_base + addresses::force_jump, false);
	}

	void force_attack()
	{
		memory.write_memory<bool>(memory.get_client().module_base + addresses::force_attack, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		memory.write_memory<bool>(memory.get_client().module_base + addresses::force_attack, false);
	}

	void set_view_angles(Vector viewangles)
	{
		memory.write_memory<Vector>(get_client_state() + offsets::viewangles, viewangles);
	}

	Vector get_view_angles()
	{
		return memory.read_memory<Vector>(get_client_state() + offsets::viewangles);
	}
	std::string get_map_dir()
	{
		static std::array< char, 0x120  > map_directory;
		map_directory = memory.read_memory< std::array< char, 0x120 > >(get_client_state() + offsets::current_map_dir);
		return map_directory.data();
	}
	std::string get_game_dir()
	{
		static std::array< char, 0x120 > game_directory;
		memory.read_into(memory.get_engine().module_base + addresses::game_dir, game_directory.data(), game_directory.size());
		return std::string(game_directory.data()).append("\\");
	}
}