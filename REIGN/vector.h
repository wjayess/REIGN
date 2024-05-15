#pragma once

struct view_matrix_t
{
	float* operator[](int index)
	{
		return matrix[index];
	}

	float matrix[4][4];
};

struct Vector3
{
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept:
		x(x), y(y), z(z) {}

	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3(x / factor, y / factor, z / factor);
	}

	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3(x * factor, y * factor, z * factor);
	}

	Vector3 WorldToScreen(view_matrix_t matrix, int screenWidth, int screenHeight) const;

	float x, y, z;
};