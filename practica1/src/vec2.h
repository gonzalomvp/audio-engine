#pragma once

#include <cmath>

#define PI 3.14159265f
#define RAD2DEG 57.2958f
#define DEG2RAD 0.0174533f

class Vec2 {
public:
	Vec2(float px = 0, float py = 0) : x(px), y(py) {}

	Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
	Vec2 operator+(float fValue)      const { return Vec2(x + fValue, y + fValue); }
	Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
	Vec2 operator-(float fValue)      const { return Vec2(x - fValue, y - fValue); }
	Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
	Vec2 operator*(float fValue)      const { return Vec2(x * fValue, y * fValue); }
	Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }
	Vec2 operator/(float fValue)      const { return Vec2(x / fValue, y / fValue); }

	Vec2  abs()                       const { return Vec2(std::abs(x), std::abs(y)); }
	float length()                    const { return sqrt(x * x + y * y); }
	Vec2  normalize()                 const { float l = length(); return (l != 0) ? *this / l : *this; }
	float dot     (const Vec2& other) const { return x * other.x + y * other.y; }
	float angle   (const Vec2& other) const { float angle = atan2(-(other.y - y), other.x - x) * RAD2DEG; return (angle >= 0) ? angle : angle + 360; }
	float distance(const Vec2& other) const { return (other - *this).length(); }

	float x;
	float y;
};
