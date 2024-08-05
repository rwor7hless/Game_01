#include "Vec2.h"

Vec2::Vec2() {};

Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {};

bool Vec2::operator == (const Vec2& rhs) const 
{
	return (this->x == rhs.x && this->y == rhs.y);
};

bool Vec2::operator != (const Vec2& rhs) const 
{
	return !(this->x == rhs.x && this->y == rhs.y);
};

Vec2 Vec2::operator + (const Vec2& rhs) const 
{
	return Vec2(this->x + rhs.x, this->y + rhs.y);
};

Vec2 Vec2::operator - (const Vec2& rhs) const 
{
	return Vec2(this->x - rhs.x, this->y - rhs.y);
};

Vec2 Vec2::operator / (const float val) const 
{
	return Vec2(this->x / val, this->y / val);
};

Vec2 Vec2::operator * (const float val) const 
{
	return Vec2(this->x * val, this->y * val);
};

void Vec2::operator += (const Vec2& rhs)  
{ 
	this->x += rhs.x;
	this->y += rhs.y;
};

void Vec2::operator -= (const Vec2& rhs)  
{
	this->x -= rhs.x;
	this->y -= rhs.y;
};

void Vec2::operator *= (const float val)  
{
	this->x *= val;
	this->y += val;
};

void Vec2::operator /= (const float val)  
{
	this->x /= val;
	this->y /= val;
};

float Vec2::dist(const Vec2& rhs) const {
	return sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
}

float Vec2::length()
{
	return sqrtf(x * x + y * y);
}

void Vec2::norm()
{
	float length = sqrtf(x * x + y * y);
	x = x / length;
	y = y / length;
}