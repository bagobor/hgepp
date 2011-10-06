/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeRect helper class implementation
 */

#include <hgerect.h>
//#include <math.h>

namespace hge
{

void hgeRect::Encapsulate(float x, float y)
{
	if (bClean)
	{
		x1 = x2 = x;
		y1 = y2 = y;
		bClean = false;
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

hgeRect::hgeRect(float _x1, float _y1, float _x2, float _y2)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
	bClean = false;
}

hgeRect::hgeRect()
{
	bClean = true;
}

void hgeRect::Set(float _x1, float _y1, float _x2, float _y2)
{
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
	bClean = false;
}

void hgeRect::SetRadius(float x, float y, float r)
{
	x1 = x - r;
	x2 = x + r;
	y1 = y - r;
	y2 = y + r;
	bClean = false;
}

} // namespace hge
