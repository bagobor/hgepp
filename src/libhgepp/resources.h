/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeResourceManager resources header
 */
#pragma once

#include <hgeresource.h>
#include "parser.h"

namespace hge {

enum
{
	RES_SCRIPT = 0,
	RES_RESOURCE = 1,
	RES_TEXTURE = 2,
	RES_EFFECT = 3,
	RES_MUSIC = 4,
	RES_STREAM = 5,
	RES_TARGET = 6,
	RES_SPRITE = 7,
	RES_ANIMATION = 8,
	RES_FONT = 9,
	RES_PARTICLE = 10,
	RES_DISTORT = 11,
	RES_STRTABLE = 12
};


//void AddRes(hgeResourceManager *rm, int type, BaseResource * resource);
//BaseResource* FindRes(hgeResourceManager *rm, int type, const char *name);



} // namespace hge
