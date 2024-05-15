#include "vector.h"

Vector3 Vector3::WorldToScreen(view_matrix_t matrix, int screenWidth, int screenHeight) const
{
	float _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
	float _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];

	float w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];

	if (w < 0.01f)
		return false;

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenWidth / 2;
	float y = screenHeight / 2;

	x += 0.5f * _x * screenWidth + 0.5f;
	y -= 0.5f * _y * screenHeight + 0.5f;

	return { x, y, w };
}