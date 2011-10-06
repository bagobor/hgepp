/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeRect helper class
 */

#pragma once

#include <hge.h>
#include <cmath>

namespace hge {

class HGE_EXPORT hgeRect
{
public:
	float x1, y1, x2, y2;

	hgeRect(float _x1, float _y1, float _x2, float _y2);
	hgeRect();

	void Clear()
	{
		bClean = true;
	}
	bool IsClean() const
	{
		return bClean;
	}
	void Set(float _x1, float _y1, float _x2, float _y2);
	void SetRadius(float x, float y, float r);
	void Encapsulate(float x, float y);

	inline bool TestPoint(float x, float y) const
	{
		if (x >= x1 && x < x2 && y >= y1 && y < y2)
			return true;
		return false;
	}

	inline bool Intersect(const hgeRect *rect) const
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
private:
	bool bClean;
};

} // namespace hge
