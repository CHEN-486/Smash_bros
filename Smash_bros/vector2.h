#ifndef VECTOR2_H
#define VECTOR2_H

#include<cmath>

class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;
	Vector2(float x,float y):x(x),y(y){}

	Vector2 operator+(const Vector2& other)const
	{
		return Vector2((this->x + other.x), (this->y + other.y));
	}
	void operator+=(const Vector2& other)
	{
		*this = *this + other;
	}
	Vector2 operator-(const Vector2& other)const
	{
		return Vector2((this->x - other.x), (this->y - other.y));
	}
	void operator-=(const Vector2& other)
	{
		*this = *this - other;
	}
	float operator*(const Vector2& other)const
	{
		return (this->x * other.x) + (this->y * other.y);
	}
	Vector2 operator*(const float& val)const
	{
		return Vector2(this->x * val, this->y * val);
	}
	void operator*=(const float& val)
	{
		*this = *this * val;
	}
	float length()
	{
		return sqrt(x * x + y * y);
	}
	Vector2 normalize()
	{
		float length = this->length();
		if (!length)
			return Vector2(0, 0);
		return Vector2(this->x / length, this->y / length);
	}

public:
	float x = 0;
	float y = 0;
};

#endif
