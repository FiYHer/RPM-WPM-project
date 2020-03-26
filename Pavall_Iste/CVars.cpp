#include "CVars.hpp"

namespace cvars
{
	weapon_config weapons[65];
	bool aimbot_enabled = true;
	bool aimbot_vischeck = true;

	bool glow_enabled = true;
	bool glow_vischeck = true;

	bool trigger_enabled = true;
	int trigger_key = 6;
	bool trigger_delay_enabled = false;
	int trigger_delay_min = 100;
	int trigger_delay_max = 150;

	bool trigger_oneshot_enabled = false;
	int trigger_oneshot_key = 5;

	bool triggerspray_enabled = true;
	int triggerspray_key = 6;
	int triggerspray_fov = 3;
	int triggerspray_bone = 8;
	extern int triggerspray_smooth = 4;
	bool triggerspray_rcs  = true;

	bool bhop_enabled = true;
}
