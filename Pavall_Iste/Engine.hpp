#pragma once
#include <cstdint>
#include <windows.h>
#include <iostream>

class Vector;
namespace engine
{
	uintptr_t get_client_state();
	void force_jump();
	void force_attack();
	void set_view_angles(Vector viewangles);
	Vector get_view_angles();
	std::string get_map_dir();
	std::string get_game_dir();
}