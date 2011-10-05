/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeRect helper class
 */

#pragma once

#include <hge.h>

namespace hge {

class HGE_EXPORT hgeRect
{
public:
	float	x1, y1, x2, y2;

	hgeRect(float _x1, float _y1, float _x2, float _y2);
	hgeRect();

	void    Clear() {bClean=true;}
	bool    IsClean() const {return bClean;}
	void	Set(float _x1, float _y1, float _x2, float _y2);
	void	SetRadius(float x, float y, float r);
	void	Encapsulate(float x, float y);
	bool	TestPoint(float x, float y) const;
	bool	Intersect(const hgeRect *rect) const;

private:
	bool	bClean;
};

} // namespace hge
