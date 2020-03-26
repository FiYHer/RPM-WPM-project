#include "Math.hpp"

#include "Include.hpp"

namespace math
{
	bool world_to_screen(Vector& from, Vector& to, int width, int height)
	{
		const auto WorldToScreenMatrix = memory.read_memory<WorldToScreenMatrix_t>(memory.get_client().module_base + addresses::viewmatrix);

		to.x = WorldToScreenMatrix.flMatrix[0][0] * from.x + WorldToScreenMatrix.flMatrix[0][1] * from.y + WorldToScreenMatrix.flMatrix[0][2] * from.z + WorldToScreenMatrix.flMatrix[0][3];
		to.y = WorldToScreenMatrix.flMatrix[1][0] * from.x + WorldToScreenMatrix.flMatrix[1][1] * from.y + WorldToScreenMatrix.flMatrix[1][2] * from.z + WorldToScreenMatrix.flMatrix[1][3];

		float w = WorldToScreenMatrix.flMatrix[3][0] * from.x + WorldToScreenMatrix.flMatrix[3][1] * from.y + WorldToScreenMatrix.flMatrix[3][2] * from.z + WorldToScreenMatrix.flMatrix[3][3];

		if (w < 0.01f)
			return false;

		float invw = 1.0f / w;
		to.x *= invw;
		to.y *= invw;

		float x = width / 2;
		float y = height / 2;

		x += 0.5 * to.x * width + 0.5;
		y -= 0.5 * to.y * height + 0.5;

		to.x = x;
		to.y = y;

		return true;
	}

	void calc_angle(Vector & vecOrigin, Vector & vecOther, Vector & vecAngles)
	{
		Vector vecDelta = Vector((vecOrigin[0] - vecOther[0]), (vecOrigin[1] - vecOther[1]), (vecOrigin[2] - vecOther[2]));
		float hyp = sqrtf(vecDelta[0] * vecDelta[0] + vecDelta[1] * vecDelta[1]);

		vecAngles[0] = (float)atan(vecDelta[2] / hyp)		*(float)(180.f / M_PI);
		vecAngles[1] = (float)atan(vecDelta[1] / vecDelta[0])	*(float)(180.f / M_PI);
		vecAngles[2] = (float)0.f;

		if (vecDelta[0] >= 0.f)
			vecAngles[1] += 180.0f;
	}

	void angle_normalize(Vector& vAngles)
	{
		for (int i = 0; i < 3; i++)
		{
			if (vAngles[i] < -180.0f) vAngles[i] += 360.0f;
			if (vAngles[i] > 180.0f) vAngles[i] -= 360.0f;
		}

		if (vAngles.x < -89.0f)
			vAngles.x = 89.0f;
		if (vAngles.x > 89.0f)
			vAngles.x = 89.0f;

		vAngles.z = 0;
	}

	void clamp_angles(Vector& vAngles)
	{
		while (vAngles.y < -180.0f)
			vAngles.y += 360.0f;
		while (vAngles.y > 180.0f)
			vAngles.y -= 360.0f;

		if (vAngles.x < -89.0f)
			vAngles.x = 89.0f;
		if (vAngles.x > 89.0f)
			vAngles.x = 89.0f;

		vAngles.z = 0;
	}

	void make_vector(Vector& vecAngle, Vector& out)
	{
		float pitch = float(vecAngle[0] * M_PI / 180);
		float tmp = float(cos(pitch));
		float yaw = float(vecAngle[1] * M_PI / 180);
		out[0] = float(-tmp * -cos(yaw));
		out[1] = float(sin(yaw)*tmp);
		out[2] = float(-sin(pitch));
	}

	float get_field_of_view(Vector& vecAngle, Vector& vecOrigin, Vector& vecOther)
	{
		Vector ang, aim;
		double fov;

		calc_angle(vecOrigin, vecOther, ang);
		make_vector(vecAngle, aim);
		make_vector(ang, ang);

		float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
		float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));

		float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

		fov = acos(u_dot_v / (mag_s*mag_d)) * (180.f / M_PI);

		fov *= 1.4;

		return float(fov);
	}
}