#pragma once
#include <cstdint>

enum CSWeaponID_t : uint16_t //enum  ItemSchemaIndex_e
{
	WEAPON_NONE = 0,

	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER
}; 

namespace weapon_generic
{
	static bool IsIDPistol(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_DEAGLE:
			return true;
		case WEAPON_ELITE:
			return true;
		case WEAPON_FIVESEVEN:
			return true;
		case WEAPON_GLOCK:
			return true;
		case WEAPON_TEC9:
			return true;
		case WEAPON_HKP2000:
			return true;
		case WEAPON_USP_SILENCER:
			return true;
		case WEAPON_P250:
			return true;
		case WEAPON_CZ75A:
			return true;
		case WEAPON_REVOLVER:
			return true;
		default:
			return false;
		}
	}

	static bool IsIDShotgun(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_XM1014:
			return true;
		case WEAPON_MAG7:
			return true;
		case WEAPON_SAWEDOFF:
			return true;
		case WEAPON_NOVA:
			return true;
		default:
			return false;
		}
	}

	static bool IsIDAutomatic(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_MP5SD:
			return true;
		case WEAPON_AK47:
			return true;
		case WEAPON_AUG:
			return true;
		case WEAPON_FAMAS:
			return true;
		case WEAPON_GALILAR:
			return true;
		case WEAPON_M249:
			return true;
		case WEAPON_M4A1:
			return true;
		case WEAPON_M4A1_SILENCER:
			return true;
		case WEAPON_MAC10:
			return true;
		case WEAPON_P90:
			return true;
		case WEAPON_UMP45:
			return true;
		case WEAPON_BIZON:
			return true;
		case WEAPON_NEGEV:
			return true;
		case WEAPON_MP7:
			return true;
		case WEAPON_MP9:
			return true;
		case WEAPON_SG556:
			return true;
		default:
			return false;
		}
	}

	static bool IsIDSniper(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_AWP:
			return true;
		case WEAPON_G3SG1:
			return true;
		case WEAPON_SCAR20:
			return true;
		case WEAPON_SSG08:
			return true;
		default:
			return false;
		}
	}

	static bool IsIDKnife(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_KNIFEGG:
			return true;
		case WEAPON_KNIFE_T:
			return true;
		case WEAPON_KNIFE_GUT:
			return true;
		case WEAPON_KNIFE_FLIP:
			return true;
		case WEAPON_BAYONET:
			return true;
		case WEAPON_KNIFE_M9_BAYONET:
			return true;
		case WEAPON_KNIFE_KARAMBIT:
			return true;
			case WEAPON_KNIFE_TACTICAL:
			return true;
		case WEAPON_KNIFE_BUTTERFLY:
			return true;
		case WEAPON_KNIFE_FALCHION:
			return true;
		case WEAPON_KNIFE_PUSH:
			return true;
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			return true;
		default:
			return false;
		}
	}

	static bool IsIDOther(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_NONE:
			return true;
		case WEAPON_TASER:
			return true;
		case WEAPON_FLASHBANG:
			return true;
		case WEAPON_HEGRENADE:
			return true;
		case WEAPON_SMOKEGRENADE:
			return true;
		case WEAPON_MOLOTOV:
			return true;
		case WEAPON_DECOY:
			return true;
		case WEAPON_INCGRENADE:
			return true;
		case WEAPON_C4:
			return true;
		default:
			return false;
		}
	}

	static const char *GetWeaponName(int m_iWeaponId)
	{
		switch (m_iWeaponId)
		{
		case WEAPON_MP5SD:
			return "MP7"; break;
		case WEAPON_DEAGLE:
			return "DEAGLE"; break;
		case WEAPON_ELITE:
			return "DUALS"; break;
		case WEAPON_FIVESEVEN:
			return "FIVE SEVEN"; break;
		case WEAPON_GLOCK:
			return "GLOCK"; break;
		case WEAPON_AK47:
			return "AK47"; break;
		case WEAPON_AUG:
			return "AUG"; break;
		case WEAPON_AWP:
			return "AWP"; break;
		case WEAPON_FAMAS:
			return "FAMAS"; break;
		case WEAPON_G3SG1:
			return "G3SG1"; break;
		case WEAPON_GALILAR:
			return "GALIL"; break;
		case WEAPON_M249:
			return "M249"; break;
		case WEAPON_M4A1:
			return "M4A1"; break;
		case WEAPON_MAC10:
			return "MAC10"; break;
		case WEAPON_P90:
			return "P90"; break;
		case WEAPON_UMP45:
			return "UMP45"; break;
		case WEAPON_XM1014:
			return "XM1014"; break;
		case WEAPON_BIZON:
			return "BIZON"; break;
		case WEAPON_MAG7:
			return "MAG7"; break;
		case WEAPON_NEGEV:
			return "NEGEV"; break;
		case WEAPON_SAWEDOFF:
			return "SAWED OFF"; break;
		case WEAPON_TEC9:
			return "TEC9"; break;
		case WEAPON_TASER:
			return "TASER"; break;
		case WEAPON_HKP2000:
			return "P2000"; break;
		case WEAPON_MP7:
			return "MP7"; break;
		case WEAPON_MP9:
			return "MP9"; break;
		case WEAPON_NOVA:
			return "NOVA"; break;
		case WEAPON_P250:
			return "P250"; break;
		case WEAPON_SCAR20:
			return "SCAR20"; break;
		case WEAPON_SG556:
			return "SG553"; break;
		case WEAPON_SSG08:
			return "SCOUT"; break;
		case WEAPON_KNIFEGG:
			return "KNIFE"; break;
		case WEAPON_KNIFE_T:
			return "KNIFE"; break;
		case WEAPON_KNIFE_GUT:
			return "GUT KNIFE"; break;
		case WEAPON_KNIFE_FLIP:
			return "FLIP KNIFE"; break;
		case WEAPON_BAYONET:
			return "BAYONET"; break;
		case WEAPON_KNIFE_KARAMBIT:
			return "KARAMBIT"; break;
		case WEAPON_KNIFE_M9_BAYONET:
			return "M9 BAYONET"; break;
		case WEAPON_KNIFE_BUTTERFLY:
			return "BUTTERFLY"; break;
		case WEAPON_KNIFE_FALCHION:
			return "FALCHION"; break;
			case WEAPON_KNIFE_TACTICAL:
				return "HUNTSMAN"; break;
		case WEAPON_KNIFE_PUSH:
			return "BUTTPLUGS"; break;
		case WEAPON_FLASHBANG:
			return "FLASHBANG"; break;
		case WEAPON_HEGRENADE:
			return "HE GRENADE"; break;
		case WEAPON_SMOKEGRENADE:
			return "SMOKE"; break;
		case WEAPON_MOLOTOV:
			return "MOLOTOV"; break;
		case WEAPON_DECOY:
			return "DECOY"; break;
		case WEAPON_INCGRENADE:
			return "INC GRENADE"; break;
		case WEAPON_M4A1_SILENCER:
			return "M4A1-S"; break;
		case WEAPON_USP_SILENCER:
			return "USP-S"; break;
		case WEAPON_CZ75A:
			return "CZ75"; break;
		case WEAPON_C4:
			return "C4"; break;
		case WEAPON_REVOLVER:
			return "COWBOY GUN"; break;
		
		default:
			return "INVALID"; break;
		}
		return "INVALID";
	}
};

class Weapon
{
private:
	uintptr_t base_weapon;

public:
	Weapon(uintptr_t base);

	bool is_valid() const;
	short get_weapon_id() const;
	
};

