#include "Config.hpp"

#include "Utils.hpp"
#include "CVars.hpp"
#include "Weapon.hpp"

Config config;

bool Config::initialize(std::string exe_path)
{
	//This gets the path to the executable so it can place a config in the same folder, if one doesnt exist
	std::string new_path = utils::remove_exe_name(exe_path);

	if (new_path.length() <= 0) //Return false if can't get the path
		return false;

	//Config location
	config_location = new_path + "config.cfg";
	memset(config_location_c_, 0x00, 255);
	memcpy(config_location_c_, config_location.c_str(), strlen(config_location.c_str()));
	load_config();
	save_config();

	//Everything worked, return true and continue in cheat
	return true;
}

void Config::read_integer(std::string section, std::string key, int& value) const
{
	value = GetPrivateProfileInt(section.c_str(), key.c_str(), value, config_location_c_);
}

void Config::read_boolean(std::string section, std::string key, bool& value) const
{
	char result[255];
	char buffer[255];
	sprintf_s(buffer, "%s", value ? "true" : "false");
	GetPrivateProfileString(section.c_str(), key.c_str(), buffer, result, 255, config_location_c_);
	value = (strcmp(result, "True") == 0 || strcmp(result, "true") == 0) ? true : false;
}

void Config::write_integer(std::string section, std::string key, int value) const
{
	char value_c_[255];
	sprintf_s(value_c_, "%d", value);
	WritePrivateProfileString(section.c_str(), key.c_str(), value_c_, config_location_c_);
}

void Config::write_boolean(std::string section, std::string key, bool value) const
{
	char value_c_[255];
	sprintf_s(value_c_, "%s", value ? "true" : "false");
	WritePrivateProfileString(section.c_str(), key.c_str(), value_c_, config_location_c_);
}

void Config::load_config() const
{
	//This just uses WriteProfileString to read from files using a key (aimbot, for example) and a value (enabled, for example)
	read_boolean("aimbot", "enabled", cvars::aimbot_enabled);
	read_boolean("glow", "enabled", cvars::glow_enabled);
	read_boolean("trigger", "enabled", cvars::trigger_enabled);
	read_integer("trigger", "key", cvars::trigger_key);
	read_boolean("bhop", "enabled", cvars::bhop_enabled);

	read_boolean("trigger_oneshot", "enabled", cvars::trigger_oneshot_enabled); //
	read_integer("trigger_oneshot", "key", cvars::trigger_oneshot_key);//

	read_boolean("triggerspray", "enabled", cvars::triggerspray_enabled);
	read_integer("triggerspray", "key", cvars::triggerspray_key);//
	read_integer("triggerspray", "fov", cvars::triggerspray_fov);//
	read_integer("triggerspray", "bone", cvars::triggerspray_bone);//
	read_integer("triggerspray", "smooth", cvars::triggerspray_smooth);//
	read_boolean("triggerspray", "rcs", cvars::triggerspray_rcs);//

	read_integer("trigger_delay_min", "value", cvars::trigger_delay_min);//
	read_integer("trigger_delay_max", "value", cvars::trigger_delay_max);//

	//Loops through weapons to make weapon config and read/write each variable to that weapon
	for (int i = 1; i < 65; i++)
	{
		/*if (!weapon_generic::IsIDPistol(i) && !weapon_generic::IsIDAutomatic(i)
			&& !weapon_generic::IsIDSniper(i) && !weapon_generic::IsIDShotgun(i))
			continue;*/

		read_boolean(weapon_generic::GetWeaponName(i), "Enabled", cvars::weapons[i].aimbot_enabled);
		read_integer(weapon_generic::GetWeaponName(i), "FOV", cvars::weapons[i].aimbot_fov);
		read_integer(weapon_generic::GetWeaponName(i), "Bone", cvars::weapons[i].aimbot_bone);
		read_integer(weapon_generic::GetWeaponName(i), "Smooth", cvars::weapons[i].aimbot_smooth);
		read_integer(weapon_generic::GetWeaponName(i), "Key", cvars::weapons[i].aimbot_key);
		read_boolean(weapon_generic::GetWeaponName(i), "RCS", cvars::weapons[i].aimbot_rcs);
	}
}

void Config::save_config() const
{
	//Same as reading, but writing
	write_boolean("aimbot", "enabled", cvars::aimbot_enabled);
	write_boolean("aimbot_vischeck", "enabled", cvars::aimbot_vischeck); //
	write_boolean("glow", "enabled", cvars::glow_enabled);
	write_boolean("trigger", "enabled", cvars::trigger_enabled);
	write_boolean("trigger_oneshot", "enabled", cvars::trigger_oneshot_enabled); //
	write_integer("trigger_oneshot", "key", cvars::trigger_oneshot_key);//

	write_boolean("triggerspray", "enabled", cvars::triggerspray_enabled);
	write_integer("triggerspray", "key", cvars::triggerspray_key);//
	write_integer("triggerspray", "fov", cvars::triggerspray_fov);//
	write_integer("triggerspray", "bone", cvars::triggerspray_bone);//
	write_integer("triggerspray", "smooth", cvars::triggerspray_smooth);//
	write_boolean("triggerspray", "rcs", cvars::triggerspray_rcs);//
	
	write_integer("trigger_delay_min", "value", cvars::trigger_delay_min);//
	write_integer("trigger_delay_max", "value", cvars::trigger_delay_max);//
	

	write_integer("trigger", "key", cvars::trigger_key);
	write_boolean("bhop", "enabled", cvars::bhop_enabled);

	//Loops through weapons to make weapon config and read/write each variable to that weapon
	for (int i = 1; i < 65; i++)
	{
		if (!weapon_generic::IsIDPistol(i) && !weapon_generic::IsIDAutomatic(i)
			&& !weapon_generic::IsIDSniper(i) && !weapon_generic::IsIDShotgun(i))
			continue;

		write_boolean(weapon_generic::GetWeaponName(i), "Enabled", cvars::weapons[i].aimbot_enabled);
		write_integer(weapon_generic::GetWeaponName(i), "FOV", cvars::weapons[i].aimbot_fov);
		write_integer(weapon_generic::GetWeaponName(i), "Bone", cvars::weapons[i].aimbot_bone);
		write_integer(weapon_generic::GetWeaponName(i), "Smooth", cvars::weapons[i].aimbot_smooth);
		write_integer(weapon_generic::GetWeaponName(i), "Key", cvars::weapons[i].aimbot_key);
		write_boolean(weapon_generic::GetWeaponName(i), "RCS", cvars::weapons[i].aimbot_rcs);
	}
}