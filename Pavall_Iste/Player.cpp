#include "Player.hpp"
#include "Include.hpp"


Player Player::get_local_player()
{
	return { memory.get_client().module_base + addresses::local_player };
}

Player Player::get_player_from_id(int id)
{
	return { memory.get_client().module_base + addresses::entity_list + (id * 0x10) };
}

Player::Player(uintptr_t address)
{
	base_player = memory.read_memory<uintptr_t>(address);
}

uintptr_t Player::get_address() const
{
	return base_player;
}

bool Player::is_valid() const
{
	return (base_player != NULL);
}

bool Player::is_valid_player() const
{
	return (is_valid() && !is_dormant() && get_life_state() == LIFE_ALIVE);
}

int Player::get_flags() const
{
	return memory.read_memory<int>(base_player + offsets::flags);
}

int Player::get_crosshair_id() const
{
	return memory.read_memory<int>(base_player + offsets::crosshairid);
}

int Player::get_team_num() const
{
	return memory.read_memory<int>(base_player + offsets::team_num);
}

bool Player::is_dormant() const
{
	return memory.read_memory<bool>(base_player + offsets::dormant);
}

char Player::get_life_state() const
{
	return memory.read_memory<char>(base_player + offsets::life_state);
}

Vector Player::get_origin() const
{
	return memory.read_memory<Vector>(base_player + offsets::origin);
}

Vector Player::get_eye_position() const
{
	const auto origin = get_origin();
	const auto offset = memory.read_memory<Vector>(base_player + offsets::viewoffset);
	return Vector(origin + offset);
}

Vector Player::get_bone_position(int bone) const
{
	const auto bone_base = memory.read_memory<uintptr_t>(base_player + offsets::bone_matrix);

	if (!bone_base)
		return Vector(0, 0, 0);

	Vector return_bone;
	return_bone.x = memory.read_memory<float>(bone_base + 0x30 * bone + 0x0C);
	return_bone.y = memory.read_memory<float>(bone_base + 0x30 * bone + 0x1C);
	return_bone.z = memory.read_memory<float>(bone_base + 0x30 * bone + 0x2C);
	return return_bone;
}

Vector Player::get_punch() const
{
	return memory.read_memory<Vector>(base_player + offsets::aim_punch);
}

int Player::get_health() const
{
	return memory.read_memory<int>(base_player + offsets::health);
}

int Player::get_class_id() const
{
	uintptr_t client_network = memory.read_memory<uintptr_t>(base_player + 0x8);
	uintptr_t get_client = memory.read_memory<uintptr_t>(client_network + 2 * 0x4);
	uintptr_t client_class = memory.read_memory<uintptr_t>(get_client + 0x1);
	int class_id = memory.read_memory<int>(client_class + 0x14);
	return class_id;
}

uintptr_t Player::get_weapon() const
{
	auto handle = memory.read_memory<uintptr_t>(base_player + offsets::active_weapon);
	auto entity = handle & 0xFFF;
	auto current_weapon = memory.read_memory<uintptr_t>(memory.get_client().module_base + addresses::entity_list + (entity - 1) * 0x10);
	return current_weapon;
}