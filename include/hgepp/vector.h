/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeVector helper class
 */
#pragma once

#include <hgepp.h>

#include <cmath>

namespace hge {

/*
** Fast 1.0/sqrtf(float) routine
*/
inline float InvSqrt(float x)
{
	union
	{
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
	return convertor.floatPart * (1.5f - 0.4999f * x * convertor.floatPart * convertor.floatPart);
}

template <class value_t>
class HGE_EXPORT vec2_t
{
public:
	typedef vec2_t<value_t> this_class_t;
	value_t x, y;

	vec2_t(value_t _x, value_t _y): x(_x), y(_y)
	{
	}
	
	vec2_t(): x(0), y(0)
	{
	}

	this_class_t operator-() const
	{
		return this_class_t(-x, -y);
	}
	
	this_class_t operator-(const this_class_t & v) const
	{
		return this_class_t(x - v.x, y - v.y);
	}
	
	this_class_t operator+(const this_class_t & v) const
	{
		return this_class_t(x + v.x, y + v.y);
	}
	
	this_class_t & operator-=(const this_class_t &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	this_class_t & operator+=(const this_class_t &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	
	bool operator==(const this_class_t & v) const
	{
		return (x == v.x && y == v.y);
	}
	
	bool operator!=(const this_class_t & v) const
	{
		return (x != v.x || y != v.y);
	}

	this_class_t operator/(const value_t scalar) const
	{
		return this_class_t(x / scalar, y / scalar);
	}
	
	this_class_t operator*(const value_t scalar) const
	{
		return this_class_t(x * scalar, y * scalar);
	}
	
	this_class_t & operator*=(const value_t scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	value_t Dot(const this_class_t & v) const
	{
		return x * v.x + y * v.y;
	}

	value_t Length() const
	{
		return (value_t)std::sqrt(Dot(*this));
	}

	void Clamp(const value_t max)
	{
		if (Length() > max)
		{
			Normalize();
			x *= max;
			y *= max;
		}
	}

	this_class_t & Normalize()
	{
		value_t rc = InvSqrt(Dot(*this));
		x *= rc;
		y *= rc;
		return *this;
	}

	inline value_t Angle() const
	{
		return (value_t)std::atan2(y, x);
	}

	value_t Angle(const this_class_t & v) const
	{
		this_class_t s = *this;
		this_class_t t = v;

		s.Normalize();
		t.Normalize();
		return (value_t)std::acos(s.Dot(t));
	}

	this_class_t & Rotate(float a)
	{
		this_class_t v;

		v.x = (value_t)(x * std::cos(a) - y * std::sin(a));
		v.y = (value_t)(x * std::sin(a) + y * std::cos(a));

		x = v.x;
		y = v.y;

		return * this;
	}
};

/*
inline hgeVector operator*(const float s, const hgeVector &v)
{
	return v * s;
}
inline float operator^(const hgeVector &v, const hgeVector &u)
{
	return v.Angle(&u);
}
inline float operator%(const hgeVector &v, const hgeVector &u)
{
	return v.Dot(&u);
}
*/

typedef vec2_t<float> hgeVector;

} // namespace hge
