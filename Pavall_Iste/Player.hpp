#pragma once
#include <cstdint>

#define FL_ONGROUND                     (1 << 0)
#define FL_DUCKING                      (1 << 1)
#define FL_WATER						(1 << 9)
#define LIFE_ALIVE						0

struct GlowObjectDefinition_t
{
	uintptr_t m_pEntity;
	float m_vecGlowColor[3];
	float m_flGlowAlpha;
	char m_unk[4];
	float m_flUnk;
	float m_flBloomAmount;
	float m_flUnk1;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	char m_unk1;
	int m_nFullBloomStencilTestValue;
	int m_nGlowStyle;
	int m_nSplitScreenSlot;
	int m_nNextFreeSlot;
};

class Vector;
class Player
{
private:
	uintptr_t base_player;

public:
	static Player get_local_player();
	static Player get_player_from_id(int id);

	Player(uintptr_t address);

	uintptr_t get_address() const;
	bool is_valid() const;
	bool is_valid_player() const;

	int get_flags() const;
	int get_crosshair_id() const;
	int get_team_num() const;
	bool is_dormant() const;
	char get_life_state() const;
	Vector get_origin() const;
	Vector get_eye_position() const;
	Vector get_bone_position(int bone) const;
	Vector get_punch() const;
	int get_health() const;
	int get_class_id() const;
	uintptr_t get_weapon() const;
};

