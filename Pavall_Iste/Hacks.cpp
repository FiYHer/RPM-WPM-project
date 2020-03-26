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

	
		
			std::cout << weapon_generic::GetWeaponName(weapon_id) , "\n";
			
			
			
		

		//Check if its not a grenade/knife, etc.
		/*if (!weapon_generic::IsIDPistol(weapon_id) && !weapon_generic::IsIDAutomatic(weapon_id)
			&& !weapon_generic::IsIDSniper(weapon_id) && !weapon_generic::IsIDShotgun(weapon_id))
			return;
		std::cout << "not a knife,grenade";*/
			
				
		//
		//Get the config for that ID weapon
		bool enabled = cvars::weapons[weapon_id].aimbot_enabled;
		int fov = cvars::weapons[weapon_id].aimbot_fov;
		int bone = cvars::weapons[weapon_id].aimbot_bone;
		int smooth = cvars::weapons[weapon_id].aimbot_smooth;
		int key = cvars::weapons[weapon_id].aimbot_key;
		bool rcs = cvars::weapons[weapon_id].aimbot_rcs;

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
		for(auto i = 1; i < 64; i++)
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

			Vector3 entity_origin3= memory.read_memory<Vector3>(entity.get_address() + offsets::origin);
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
			if(current_fov < best_fov && current_fov <= fov)
			{
				//If it is, set this target as the best, until a new better target found
				best_fov = current_fov;
				best_target = i;
				best_position = bone_position;
			}
		}

		//If it found a target inside FOV
		if(best_target != -1)
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
			if (rcs)
				aim_pos -= local_player.get_punch() * 2.f;

			//If smooth enabled
			if(smooth > 0)
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
		//Get local player
		auto local_player = Player::get_local_player();

		//Check if local player valid
		if (!local_player.is_valid())
			return;

		//Get the glow array that's in csgo by default
		uintptr_t glow_array = memory.read_memory<uintptr_t>(memory.get_client().module_base + addresses::glow_object);
		//Get the glow array count (length)
		int glow_count = memory.read_memory<int>(memory.get_client().module_base + addresses::glow_object + 0x4);
	
		//Loop through glow objects
		for (int i = 0; i < glow_count; i++)
		{
			//Get the glow object at i
			GlowObjectDefinition_t glow_object = memory.read_memory<GlowObjectDefinition_t>(glow_array + 0x38 * i);
			
			//Check if the entity is valid
			if (!glow_object.m_pEntity)
				continue;

			//Check if the entity is dormant
			bool is_dormant = memory.read_memory<bool>(glow_object.m_pEntity + offsets::dormant);
			if (is_dormant)
				continue;

			//This gets the class id of the entity
			uintptr_t client_network = memory.read_memory<uintptr_t>(glow_object.m_pEntity + 0x8);
			uintptr_t get_client = memory.read_memory<uintptr_t>(client_network + 2 * 0x4);
			uintptr_t client_class = memory.read_memory<uintptr_t>(get_client + 0x1);
			int class_id = memory.read_memory<int>(client_class + 0x14); 
			
			//If it's not 35 (CS_PLAYER), continue to next entity
			if (class_id != 40)
				continue;

			//Check the team against the local players
			int team_num = memory.read_memory<int>(glow_object.m_pEntity + offsets::team_num);
			if (team_num == local_player.get_team_num())
				continue;

			Vector3 entity_origin = memory.read_memory<Vector3>(glow_object.m_pEntity + offsets::origin);
			Vector3 entity_offset = memory.read_memory<Vector3>(glow_object.m_pEntity + offsets::viewoffset);
			Vector3 entity_eye_pos = Vector3(entity_origin + entity_offset);

			Vector3 local_origin = memory.read_memory<Vector3>(local_player.get_address() + offsets::origin);
			Vector3 local_offset = memory.read_memory<Vector3>(local_player.get_address() + offsets::viewoffset);
			Vector3 local_eye_pos = Vector3(local_origin + local_offset);

			bool is_visible = bsp_parser->is_visible(local_eye_pos, entity_eye_pos);

			//Setup colors
			float color[3] = { 1, 1, 1 };

			//If it's a terrorist make it red, if it's a counter-terrorist make it blue
			if (team_num == 2) {
				color[0] = 1;
				color[1] = 0;
				color[2] = 0;
				if (is_visible && cvars::glow_vischeck)
					color[2] = 1;
			}
			else if (team_num == 3)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 1;
				if (is_visible && cvars::glow_vischeck)
					color[1] = 1;
			}

			//Setup the object
			glow_object.m_vecGlowColor[0] = color[0];
			glow_object.m_vecGlowColor[1] = color[1];
			glow_object.m_vecGlowColor[2] = color[2];
			glow_object.m_flGlowAlpha = 0.5f;
			glow_object.m_bRenderWhenOccluded = true;
			glow_object.m_bRenderWhenUnoccluded = false;

			//Write to memory
			memory.write_memory<GlowObjectDefinition_t>(glow_array + (i * 0x38), glow_object);
		}
	}

	void bhop()
	{
		//Check if you're holding bhop key, if not, ignore rest of function
		if (!(GetAsyncKeyState(VK_SPACE) & 0x8000))
			return;

		//Get local player
		auto local_player = Player::get_local_player();

		//Check if local player valid
		if (!local_player.is_valid())
			return;

		//Check if you're on the ground and jump if so
		if (local_player.get_flags() & FL_ONGROUND)
			engine::force_jump(); 
	}

	void trigger()
	{
		//If you don't hold trigger key, ignore rest of function
		static bool is_holding = false;
		static bool set_delay = false;
		
		if (!(GetAsyncKeyState(cvars::trigger_key) & 0x8000))
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
		if(cross_id > 0 && cross_id < 65)
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
				
			}
		}
	}

	void trigger_oneshot()
	{
		static bool is_holding = false;
		static bool has_shot = false;

		if (!(GetAsyncKeyState(cvars::trigger_oneshot_key) & 0x8000))
		{
			is_holding = false;
			has_shot = false;
		}
		else
			is_holding = true;

		auto local_player = Player::get_local_player();

		if (!local_player.is_valid())
			return;

		const auto cross_id = local_player.get_crosshair_id();
		if (cross_id > 0 && cross_id < 65)
		{
			auto aiming_at = Player::get_player_from_id(cross_id - 1);

			if (!aiming_at.is_valid_player())
				return;

			if (aiming_at.get_team_num() == local_player.get_team_num())
				return;

			if (is_holding && !has_shot) {
				engine::force_attack();
				has_shot = true;
			}
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
		while(true)
		{
			std::string game_dir = engine::get_game_dir();
			std::string map_dir = engine::get_map_dir();

			if (bsp_parser->parse_map(game_dir, map_dir)) {
				static std::string last_map;
				auto bsp_file = bsp_parser->get_bsp();
				if (last_map != bsp_file.m_FileName) {
					last_map = bsp_file.m_FileName;
				}
			}

			if(cvars::aimbot_enabled) {
				aimbot();
			}

			if(cvars::bhop_enabled) {
				bhop();
			}

			if(cvars::trigger_enabled) {
				trigger();
			}

			if (cvars::trigger_oneshot_enabled) {
				trigger_oneshot();
			}

			if (cvars::triggerspray_enabled) {
				triggerspray();
			}

			if (cvars::glow_enabled) {
				glow();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}
