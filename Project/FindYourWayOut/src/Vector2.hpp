#pragma once

#pragma region include system
#include <math.h>
#pragma endregion

/// <summary>
/// vector 2 class with x and y values
/// </summary>
struct SVector2
{
public:
	#pragma region constructor
	/// <summary>
	/// constructor
	/// </summary>
	SVector2() {}

	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="_x">x value</param>
	/// <param name="_y">y value</param>
	SVector2(float _x, float _y) : X(_x), Y(_y) {}
	#pragma endregion

	#pragma region method
	/// <summary>
	/// get squared length of vector 2
	/// </summary>
	/// <returns>squared length of vector 2</returns>
	constexpr float LengthSquared() const noexcept
	{
		return X * X + Y * Y;
	}
	
	/// <summary>
	/// get length of vector 2
	/// </summary>
	/// <returns>length of vector 2</returns>
	inline const float Length() const noexcept
	{
		return sqrt(LengthSquared());
	}

	/// <summary>
	/// normalize vector 2
	/// </summary>
	inline void Normalize()
	{
		// get length of vector 2
		float length = Length();

		// calculate x and y based on length
		X /= length;
		Y /= length;
	}

	/// <summary>
	/// get normalized vector 2
	/// </summary>
	inline const SVector2 Normalized() const
	{
		// get length of vector 2
		float length = Length();

		// return calculated x and y based on length
		return SVector2(X / length, Y / length);
	}

	/// <summary>
	/// dot product to other vector
	/// </summary>
	/// <param name="_other">other vector</param>
	/// <returns>dot rpoduct</returns>
	inline const float Dot(SVector2 _other) const
	{
		return X * _other.X + Y * _other.Y;
	}
	#pragma endregion

	#pragma region operator
	/// <summary>
	/// operator +
	/// </summary>
	/// <param name="_other">other vector</param>
	/// <returns>calculated vector</returns>
	inline const SVector2 operator+(SVector2 _other) const
	{
		return { X + _other.X, Y + _other.Y };
	}

	/// <summary>
	/// operator -
	/// </summary>
	/// <param name="_other">other vector</param>
	/// <returns>calculated vector</returns>
	inline const SVector2 operator-(SVector2 _other) const
	{
		return SVector2(X - _other.X, Y - _other.Y);
	}

	/// <summary>
	/// operator *
	/// </summary>
	/// <param name="_value">value to multiply</param>
	/// <returns>calculated vector</returns>
	inline const SVector2 operator*(float _value) const
	{
		return SVector2(X * _value, Y * _value);
	}
	#pragma endregion

	#pragma region primitive variables
	/// <summary>
	/// x value
	/// </summary>
	float X{ 0.0f };

	/// <summary>
	/// y value
	/// </summary>
	float Y{ 0.0f };
	#pragma endregion
};