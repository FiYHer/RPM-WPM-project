#pragma once

class Vector;
namespace math
{
	typedef struct {
		float flMatrix[4][4];
	}WorldToScreenMatrix_t;

	bool world_to_screen(Vector& from, Vector& to, int width, int height);
	void calc_angle(Vector& vecOrigin, Vector& vecOther, Vector& vecAngles);
	void angle_normalize(Vector& vAngles);
	void clamp_angles(Vector& vAngles);
	void make_vector(Vector& vecAngle, Vector& out);
	float get_field_of_view(Vector& vecAngle, Vector& vecOrigin, Vector& vecOther);
}