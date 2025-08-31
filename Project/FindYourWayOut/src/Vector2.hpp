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
	inline float LengthSquared()
	{
		return X * X + Y * Y;
	}
	
	/// <summary>
	/// get length of vector 2
	/// </summary>
	/// <returns>length of vector 2</returns>
	inline float Length()
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
	inline SVector2 Normalized()
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
	inline float Dot(const SVector2& _other)
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
	inline SVector2 operator+(const SVector2& _other)
	{
		return SVector2(X + _other.X, Y + _other.Y);
	}

	/// <summary>
	/// operator -
	/// </summary>
	/// <param name="_other">other vector</param>
	/// <returns>calculated vector</returns>
	inline SVector2 operator-(const SVector2& _other)
	{
		return SVector2(X - _other.X, Y - _other.Y);
	}

	/// <summary>
	/// operator *
	/// </summary>
	/// <param name="_value">value to multiply</param>
	/// <returns>calculated vector</returns>
	inline SVector2 operator*(const float& _value)
	{
		return SVector2(X * _value, Y * _value);
	}
	#pragma endregion

	#pragma region primitive variables
	/// <summary>
	/// x value
	/// </summary>
	float X = 0;

	/// <summary>
	/// y value
	/// </summary>
	float Y = 0;
	#pragma endregion
};