///////
//DO NOT REMOVE THIS INFORMATION
//Author: Emilian Cioca
//
//USE OF THIS FILE IMPLIES AGREEMENT TO THE FOLLOWING TERM
//1. THIS FILE IS NOT TO BE USED FOR A GDW(game development workshop) PROJECT
//
///////

#pragma once

struct vec2
{
	vec2() = default;
	vec2(float x, float y);
	explicit vec2(float val);

	bool operator==(const vec2&) const;
	bool operator!=(const vec2&) const;

	vec2& operator-=(const vec2&);
	vec2& operator+=(const vec2&);
	vec2& operator*=(const vec2&);
	vec2& operator/=(const vec2&);
	vec2& operator*=(float scalar);
	vec2& operator/=(float divisor);

	vec2 operator-() const;
	vec2 operator-(const vec2&) const;
	vec2 operator+(const vec2&) const;
	vec2 operator*(const vec2&) const;
	vec2 operator/(const vec2&) const;
	vec2 operator*(float scalar) const;
	vec2 operator/(float divisor) const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	float Length() const;
	float LengthSquared() const;
	void ClampLength(float length);
	void Normalize();
	vec2 GetNormalized() const;

	static const vec2 Zero;
	static const vec2 One;
	static const vec2 Right;
	static const vec2 Up;

	float x = 0.0f;
	float y = 0.0f;
};

struct vec3
{
	vec3() = default;
	vec3(const vec2& xy, float z);
	vec3(float x, float y, float z);
	explicit vec3(float val);

	bool operator==(const vec3&) const;
	bool operator!=(const vec3&) const;

	vec3& operator-=(const vec3&);
	vec3& operator+=(const vec3&);
	vec3& operator*=(const vec3&);
	vec3& operator/=(const vec3&);
	vec3& operator*=(float scalar);
	vec3& operator/=(float divisor);

	vec3 operator-() const;
	vec3 operator-(const vec3&) const;
	vec3 operator+(const vec3&) const;
	vec3 operator*(const vec3&) const;
	vec3 operator/(const vec3&) const;
	vec3 operator*(float scalar) const;
	vec3 operator/(float divisor) const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	explicit operator vec2() const;

	float Length() const;
	float LengthSquared() const;
	void ClampLength(float length);
	void Normalize();
	vec3 GetNormalized() const;

	static const vec3 Zero;
	static const vec3 One;
	static const vec3 Right;
	static const vec3 Up;
	static const vec3 Forward;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct vec4
{
	vec4() = default;
	vec4(const vec3& xyz, float w);
	vec4(const vec2& xy, float y, float w);
	vec4(const vec2& xy, const vec2& zw);
	vec4(float x, float y, float z, float w);
	explicit vec4(float val);

	bool operator==(const vec4&) const;
	bool operator!=(const vec4&) const;

	vec4& operator-=(const vec4&);
	vec4& operator+=(const vec4&);
	vec4& operator*=(const vec4&);
	vec4& operator/=(const vec4&);
	vec4& operator*=(float scalar);
	vec4& operator/=(float divisor);

	vec4 operator-() const;
	vec4 operator-(const vec4&) const;
	vec4 operator+(const vec4&) const;
	vec4 operator*(const vec4&) const;
	vec4 operator/(const vec4&) const;
	vec4 operator*(float scalar) const;
	vec4 operator/(float divisor) const;

	float operator[](unsigned index) const;
	float& operator[](unsigned index);

	explicit operator vec2() const;
	explicit operator vec3() const;

	float Length() const;
	float LengthSquared() const;
	void ClampLength(float length);
	void Normalize();
	vec4 GetNormalized() const;

	static const vec4 Zero;
	static const vec4 One;
	static const vec4 Right;
	static const vec4 Up;
	static const vec4 Forward;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

float Distance(const vec2&, const vec2&);
float Distance(const vec3&, const vec3&);
float Distance(const vec4&, const vec4&);
float Dot(const vec2&, const vec2&);
float Dot(const vec3&, const vec3&);
float Dot(const vec4&, const vec4&);
vec3 Cross(const vec3&, const vec3&);
vec2 Reflect(const vec2& incident, const vec2& normal);
vec3 Reflect(const vec3& incident, const vec3& normal);
vec4 Reflect(const vec4& incident, const vec4& normal);
vec2 Refract(const vec2& incident, const vec2& normal, float index);
vec3 Refract(const vec3& incident, const vec3& normal, float index);
vec4 Refract(const vec4& incident, const vec4& normal, float index);

vec2 operator*(float scalar, const vec2&);
vec2 operator/(float divisor, const vec2&);
vec3 operator*(float scalar, const vec3&);
vec3 operator/(float divisor, const vec3&);
vec4 operator*(float scalar, const vec4&);
vec4 operator/(float divisor, const vec4&);
