/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeRect helper class
 */

#pragma once

#include <hgepp.h>
#include <cmath>

namespace hge {

template <class value_t>
class rect_t
{
public:
	value_t		x1, y1, x2, y2;
public:
	typedef rect_t<value_t> this_class_t;

	rect_t()
		: x1(0), y1(0), x2(0), y2(0)
	{
	}
	rect_t(value_t _x1, value_t _y1, value_t _x2, value_t _y2)
		: x1(_x1), y1(_y1), x2(_x2), y2(_y2)
	{
	}

	inline void Set(value_t _x1, value_t _y1, value_t _x2, value_t _y2)
	{
		x1 = _x1;
		x2 = _x2;
		y1 = _y1;
		y2 = _y2;
	}

	inline void SetRadius(value_t x, value_t y, value_t r)
	{
		x1 = x - r;
		x2 = x + r;
		y1 = y - r;
		y2 = y + r;
	}

	inline bool TestPoint(value_t x, value_t y) const
	{
		return (x >= x1 && x < x2 && y >= y1 && y < y2);
	}

	template <class value2_t>
	inline bool Intersect(const rect_t<value2_t> & rect) const
	{
		if (std::fabs(x1 + x2 - rect->x1 - rect->x2) < (x2 - x1 + rect->x2 - rect->x1))
		{
			if (std::fabs(y1 + y2 - rect->y1 - rect->y2) < (y2 - y1 + rect->y2 - rect->y1))
			{
				return true;
			}
		}
		return false;
	}
};

class HGE_EXPORT hgeRect: public rect_t<float>
{
// public:
// 	float x1, y1, x2, y2;

private:
	bool m_clean;

public:

	hgeRect(float _x1, float _y1, float _x2, float _y2)
		: this_class_t(_x1,_y1,_x2,_y2), m_clean(false)
	{
	}

	hgeRect()
		: this_class_t(), m_clean(true)
	{
	}

	void Clear()
	{
		m_clean = true;
	}

	bool IsClean() const
	{
		return m_clean;
	}

	inline void Encapsulate(float x, float y)
	{
		if (m_clean)
		{
			x1 = x2 = x;
			y1 = y2 = y;
			m_clean = false;
		}
		else
		{
		if (x < x1)
			x1 = x;
		if (x > x2)
			x2 = x;
		if (y < y1)
			y1 = y;
		if (y > y2)
			y2 = y;
 		}
	}

	inline void Set(float _x1, float _y1, float _x2, float _y2)
	{
		x1 = _x1;
		x2 = _x2;
		y1 = _y1;
		y2 = _y2;
		m_clean = false;
	}

	inline void SetRadius(float x, float y, float r)
	{
		x1 = x - r;
		x2 = x + r;
		y1 = y - r;
		y2 = y + r;
		m_clean = false;
	}
};

} // namespace hge
