#include "BSPParser.hpp"
#include "Hacks.hpp"

std::unique_ptr< Valve::BSPParser > bsp_parser = std::make_unique< Valve::BSPParser >();
using Vector3 = Matrix< float, 3, 1 >;

namespace hacks
{
	void aimbot()
	{
		//Get the local player
		auto local_player = Player::get_local_player();
		if (!local_player.is_valid()) return;

		//Get the weapon of the local player
		//Weapon weapon = Weapon(local_player.get_weapon());
		//if (!weapon.is_valid()) return;

		//Get the weapon id
		//int weapon_id = weapon.get_weapon_id();

		//Get local player view angles and eye position
		const auto local_player_view_angles = engine::get_view_angles();
		auto local_eye_pos = local_player.get_eye_position();

		//Setup some variables to find the enemy closest to FOV
		auto best_fov = 360.f;
		auto best_target = -1;
		auto best_position = Vector(0, 0, 0);

		//Loop through all players
		for (auto i = 1; i < 64; i++)
		{
			//Get player at i
			auto entity = Player::get_player_from_id(i);
			if (!entity.is_valid_player()) continue;

			//Check if its a teammate
			if (entity.get_team_num() == local_player.get_team_num()) continue;

			//Check if its a valid player
			if (entity.get_class_id() != 38) continue;

			//Get the bone position of the bone thats set to that weapon
			auto bone_position = entity.get_bone_position(8);

			//Check if the bone position is NULL
			if (bone_position == Vector(0, 0, 0)) continue;

			Vector3 entity_origin3 = memory.read_memory<Vector3>(entity.get_address() + offsets::origin);
			Vector3 entity_offset3 = memory.read_memory<Vector3>(entity.get_address() + offsets::viewoffset);
			Vector3 entity_eye_pos3 = Vector3(entity_origin3 + entity_offset3);

			Vector3 local_origin3 = memory.read_memory<Vector3>(local_player.get_address() + offsets::origin);
			Vector3 local_offset3 = memory.read_memory<Vector3>(local_player.get_address() + offsets::viewoffset);
			Vector3 local_eye_pos3 = Vector3(local_origin3 + local_offset3);

			bool is_visible = bsp_parser->is_visible(local_eye_pos3, entity_eye_pos3);
			if (!is_visible) continue;

			//Get viewangles + recoil control
			auto view_angles_rcs = local_player_view_angles + local_player.get_punch() * 2.f;
			//Get the current FOV from the player crosshair + recoil control to the enemy
			const auto current_fov = math::get_field_of_view(view_angles_rcs, local_eye_pos, bone_position);
			//Check if the player in the loop is closer than the last player it checked and that is inside the FOV that's set in the weapon config
			if (current_fov < best_fov && current_fov <= 10.f)
			{
				//If it is, set this target as the best, until a new better target found
				best_fov = current_fov;
				best_target = i;
				best_position = bone_position;
			}
		}

		//If it found a target inside FOV
		if (best_target != -1)
		{
			//Get that entity
			auto target = Player::get_player_from_id(best_target);
			if (!target.is_valid_player())return;

			//Calculate the angle you need to set the viewangles to go to that person
			Vector aim_pos;
			math::calc_angle(local_eye_pos, best_position, aim_pos);

			//If recoil control is enabled, then take the angle and subract the recoil control
			aim_pos -= local_player.get_punch() * 2.f;

			//If smooth enabled
			int smooth = -1;
			if (smooth > 0)
			{
				//Do some math to smooth it out, pretty simple stuff
				auto delta = local_player_view_angles - aim_pos;
				math::angle_normalize(delta);
				aim_pos.x = local_player_view_angles.x - delta.x / smooth;
				aim_pos.y = local_player_view_angles.y - delta.y / smooth;
			}

			//Normalize and clamp to avoid untrusted ban
			math::angle_normalize(aim_pos);
			math::clamp_angles(aim_pos);

			//Set viewangles
			engine::set_view_angles(aim_pos);
		}
	}

	void glow()
	{
		//获取玩家
		auto local_player = Player::get_local_player();
		if (local_player.is_valid() == false) return;

		//获取辉光管理器
		DWORD glow_manager = memory.read_memory<DWORD>(memory.get_client().module_base + addresses::glow_object);
		if (glow_manager == 0) return;

		for (int i = 0; i < 64; i++)
		{
			//获取每一个玩家
			auto entity = Player::get_player_from_id(i);
			if (entity.is_valid_player() == false || local_player.get_address() == entity.get_address()) continue;

			//获取辉光索引
			int glow_index = memory.read_memory<int>(entity.get_address() + offsets::glow_index);

			//敌人
			Vector3 entity_origin3 = memory.read_memory<Vector3>(entity.get_address() + offsets::origin);
			Vector3 entity_offset3 = memory.read_memory<Vector3>(entity.get_address() + offsets::viewoffset);
			Vector3 entity_eye_pos3 = Vector3(entity_origin3 + entity_offset3);

			//自己
			Vector3 local_origin3 = memory.read_memory<Vector3>(local_player.get_address() + offsets::origin);
			Vector3 local_offset3 = memory.read_memory<Vector3>(local_player.get_address() + offsets::viewoffset);
			Vector3 local_eye_pos3 = Vector3(local_origin3 + local_offset3);

			//墙体判断
			bool is_visible = bsp_parser->is_visible(local_eye_pos3, entity_eye_pos3);

			{
				float value[4]{ 0,0,2.0f,0.5f };
				if (is_visible) value[2] = 0.0f;
				if (local_player.get_team_num() == entity.get_team_num()) value[1] = 2.0f;
				else value[0] = 2.0f;

				memory.write_memory<float>(glow_manager + ((glow_index * 0x38) + 0x4), value[0]);
				memory.write_memory<float>(glow_manager + ((glow_index * 0x38) + 0x8), value[1]);
				memory.write_memory<float>(glow_manager + ((glow_index * 0x38) + 0xc), value[2]);
				memory.write_memory<float>(glow_manager + ((glow_index * 0x38) + 0x10), value[3]);
			}

			{
				bool value[2]{ true,false };
				memory.write_memory<bool>(glow_manager + ((glow_index * 0x38) + 0x24), value[0]);
				memory.write_memory<bool>(glow_manager + ((glow_index * 0x38) + 0x25), value[1]);
			}
		}
	}

	void bhop()
	{
		//Get local player
		auto local_player = Player::get_local_player();
		if (!local_player.is_valid())return;

		//Check if you're on the ground and jump if so
		if (local_player.get_flags() & FL_ONGROUND) engine::force_jump();
	}

	void trigger()
	{
		//Get local player
		auto local_player = Player::get_local_player();
		if (!local_player.is_valid()) return;

		//Get player ID of the person in your crosshair
		const auto cross_id = local_player.get_crosshair_id();

		//Check if there even is a player in your crosshair
		if (cross_id > 0 && cross_id < 65)
		{
			//Get the player entity that you are aiming at
			auto aiming_at = Player::get_player_from_id(cross_id - 1);
			if (!aiming_at.is_valid_player()) return;

			//Check it's not a teammate
			if (aiming_at.get_team_num() == local_player.get_team_num()) return;

			//Shoot
		}
	}

	void trigger_oneshot()
	{
		auto local_player = Player::get_local_player();
		if (!local_player.is_valid())return;

		const auto cross_id = local_player.get_crosshair_id();
		if (cross_id > 0 && cross_id < 65)
		{
			auto aiming_at = Player::get_player_from_id(cross_id - 1);
			if (!aiming_at.is_valid_player())return;

			if (aiming_at.get_team_num() == local_player.get_team_num())return;

			engine::force_attack();
		}
	}

	void triggerspray()
	{
		//If you don't hold trigger key, ignore rest of function
		static bool is_holding = false;
		static bool set_delay = false;

		if (!(GetAsyncKeyState(cvars::triggerspray_key) & 0x8000))
		{
			is_holding = false;
			set_delay = false;
		}
		else
			is_holding = true;

		//Get local player
		auto local_player = Player::get_local_player();

		//Check if your player is valid
		if (!local_player.is_valid())
			return;

		//Get player ID of the person in your crosshair
		const auto cross_id = local_player.get_crosshair_id();
		//Check if there even is a player in your crosshair
		if (cross_id > 0 && cross_id < 65)
		{
			//Get the player entity that you are aiming at
			auto aiming_at = Player::get_player_from_id(cross_id - 1);

			//Check if that player is valid
			if (!aiming_at.is_valid_player())
				return;

			//Check it's not a teammate
			if (aiming_at.get_team_num() == local_player.get_team_num())
				return;

			//Shoot
			if (is_holding)
			{
				//Get the local player
				auto local_player = Player::get_local_player();

				//Check if the local player is valid
				if (!local_player.is_valid())
					return;

				//Get the weapon of the local player
				Weapon weapon = Weapon(local_player.get_weapon());

				//Check if that is valid
				if (!weapon.is_valid())
					return;

				//Get the weapon id
				int weapon_id = weapon.get_weapon_id();
				//std::cout << weapon_id;

				//std::cout << weapon_generic::GetWeaponName(weapon_id), "\n";

				//Check if its not a grenade/knife, etc.
				/*if (!weapon_generic::IsIDPistol(weapon_id) && !weapon_generic::IsIDAutomatic(weapon_id)
				&& !weapon_generic::IsIDSniper(weapon_id) && !weapon_generic::IsIDShotgun(weapon_id))
				return;
				std::cout << "not a knife,grenade";*/

				//
				//Get the config for that ID weapon
				bool enabled = cvars::triggerspray_enabled;
				int fov = cvars::triggerspray_fov;
				int bone = cvars::triggerspray_bone;
				int smooth = cvars::triggerspray_smooth;
				int key = cvars::triggerspray_key;
				bool rcs = cvars::triggerspray_rcs;

				//Check if aimbot is enabled for that weapon
				if (!enabled)
					return;

				//Check the aimkey of that weapon
				if (!(GetAsyncKeyState(key) & 0x8000))
					return;

				//Get local player view angles and eye position
				const auto local_player_view_angles = engine::get_view_angles();
				auto local_eye_pos = local_player.get_eye_position();

				//Setup some variables to find the enemy closest to FOV
				auto best_fov = 360.f;
				auto best_target = -1;
				auto best_position = Vector(0, 0, 0);

				//Loop through all players
				for (auto i = 1; i < 64; i++)
				{
					//Get player at i
					auto entity = Player::get_player_from_id(i);

					//Check if that entity is a valid player
					if (!entity.is_valid_player())
						continue;

					//Check if its a teammate
					if (entity.get_team_num() == local_player.get_team_num())
						continue;

					//Check if its a valid player
					if (entity.get_class_id() != 38)
						continue;

					//Get the bone position of the bone thats set to that weapon
					auto bone_position = entity.get_bone_position(bone);

					//Check if the bone position is NULL
					if (bone_position == Vector(0, 0, 0))
						continue;

					Vector3 entity_origin3 = memory.read_memory<Vector3>(entity.get_address() + offsets::origin);
					Vector3 entity_offset3 = memory.read_memory<Vector3>(entity.get_address() + offsets::viewoffset);
					Vector3 entity_eye_pos3 = Vector3(entity_origin3 + entity_offset3);

					Vector3 local_origin3 = memory.read_memory<Vector3>(local_player.get_address() + offsets::origin);
					Vector3 local_offset3 = memory.read_memory<Vector3>(local_player.get_address() + offsets::viewoffset);
					Vector3 local_eye_pos3 = Vector3(local_origin3 + local_offset3);

					bool is_visible = bsp_parser->is_visible(local_eye_pos3, entity_eye_pos3);
					if (cvars::aimbot_vischeck)
						if (!is_visible)
							continue;

					//Get viewangles + recoil control
					auto view_angles_rcs = local_player_view_angles + local_player.get_punch() * 2.f;
					//Get the current FOV from the player crosshair + recoil control to the enemy
					const auto current_fov = math::get_field_of_view(view_angles_rcs, local_eye_pos, bone_position);
					//Check if the player in the loop is closer than the last player it checked and that is inside the FOV that's set in the weapon config
					if (current_fov < best_fov && current_fov <= fov)
					{
						//If it is, set this target as the best, until a new better target found
						best_fov = current_fov;
						best_target = i;
						best_position = bone_position;
					}
				}

				//If it found a target inside FOV
				if (best_target != -1)
				{
					//Get that entity
					auto target = Player::get_player_from_id(best_target);

					//Check if valid
					if (!target.is_valid_player())
						return;

					//Calculate the angle you need to set the viewangles to go to that person
					Vector aim_pos;
					math::calc_angle(local_eye_pos, best_position, aim_pos);

					//If recoil control is enabled, then take the angle and subract the recoil control
					//if (rcs)
					aim_pos -= local_player.get_punch() * 2.f;

					//If smooth enabled
					if (smooth > 0)
					{
						//Do some math to smooth it out, pretty simple stuff
						auto delta = local_player_view_angles - aim_pos;
						math::angle_normalize(delta);
						aim_pos.x = local_player_view_angles.x - delta.x / smooth;
						aim_pos.y = local_player_view_angles.y - delta.y / smooth;
					}

					//Normalize and clamp to avoid untrusted ban
					math::angle_normalize(aim_pos);
					math::clamp_angles(aim_pos);

					//Set viewangles
					engine::set_view_angles(aim_pos);
				}
			}
		}
	}

	void main_thread()
	{
		//Loop to run all the cheats
		while (true)
		{
			std::string game_dir = engine::get_game_dir();
			std::string map_dir = engine::get_map_dir();

			if (bsp_parser->parse_map(game_dir, map_dir))
			{
				static std::string last_map;
				auto bsp_file = bsp_parser->get_bsp();
				if (last_map != bsp_file.m_FileName) {
					last_map = bsp_file.m_FileName;
				}
			}

			if (GetAsyncKeyState(VK_LBUTTON)) aimbot();

			bhop();
			trigger_oneshot();
			glow();
		}
	}
}