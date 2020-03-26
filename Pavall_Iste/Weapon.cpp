#include "Weapon.hpp"
#include "Include.hpp"

Weapon::Weapon(uintptr_t base)
{
	base_weapon = base;
}

bool Weapon::is_valid() const
{
	return (base_weapon != NULL);
}

short Weapon::get_weapon_id() const
{
	return memory.read_memory<short>(base_weapon + offsets::item_index);
}