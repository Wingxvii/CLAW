///////
//DO NOT REMOVE THIS INFORMATION
//Author: Emilian Cioca
//
//USE OF THIS FILE IMPLIES AGREEMENT TO THE FOLLOWING TERM
//1. THIS FILE IS NOT TO BE USED FOR A GDW(game development workshop) PROJECT
//
///////

#pragma once

#define PI 3.14159265358979323846f

float ToRadians(float degrees);
float ToDegrees(float radians);

template<class T>
// Clamps data to the range [min, max]
T Clamp(const T &data, const T &min, const T &max)
{
	if (data > max)
	{
		return max;
	}
	else if (data < min)
	{
		return min;
	}
	else
	{
		return data;
	}
}

// Linear interpolation
template<class T>
T LERP(const T &data1, const T &data2, float u)
{
	return data1 * (1.0f - u) + data2 * u;
}
