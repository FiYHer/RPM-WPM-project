#pragma once

namespace cvars
{
	struct weapon_config {
		bool aimbot_enabled = true;
		int aimbot_fov = 3;
		int aimbot_bone = 8;
		int aimbot_smooth = 5;
		int aimbot_key = 1;
		bool aimbot_rcs = true;
	};
	extern weapon_config weapons[65];
	extern bool aimbot_enabled;
	extern bool aimbot_vischeck;

	extern bool glow_enabled;
	extern bool glow_vischeck;

	extern bool trigger_enabled;
	extern int trigger_key;
	extern bool trigger_delay_enabled;
	extern int trigger_delay_min;
	extern int trigger_delay_max;

	extern bool trigger_oneshot_enabled;
	extern int trigger_oneshot_key;

	extern bool triggerspray_enabled;
	extern int triggerspray_key;
	extern int triggerspray_fov;
	extern int triggerspray_bone;
	extern int triggerspray_smooth;
	extern bool triggerspray_rcs;

	extern bool bhop_enabled;
}
