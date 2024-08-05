#pragma once
#ifndef VEC2_H
#define VEC2_H
#include <math.h>
#include <iostream>
class Vec2
{
public:
	float x = 0;
	float y = 0;
	Vec2();
	Vec2(float _x, float _y);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const float val) const;
	Vec2 operator * (const float val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);

	float dist(const Vec2& rhs) const;
	float length();
	void norm();

};

#endif // !VEC2_H



