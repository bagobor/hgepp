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


void AddRes(hgeResourceManager *rm, int type, ResDesc *resource);
ResDesc* FindRes(hgeResourceManager *rm, int type, const char *name);


struct RScript: public ResDesc
{
	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm)
	{
		return 0;
	}
	virtual void Free()
	{
	}
};

struct RResource: public ResDesc
{
	char filename[MAXRESCHARS];

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RTexture: public ResDesc
{
	char filename[MAXRESCHARS];
	bool mipmap;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct REffect: public ResDesc
{
	char filename[MAXRESCHARS];

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RMusic: public ResDesc
{
	char filename[MAXRESCHARS];
	int amplify;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RStream: public ResDesc
{
	char filename[MAXRESCHARS];

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RTarget: public ResDesc
{
	int width;
	int height;
	bool zbuffer;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RSprite: public ResDesc
{
	char texname[MAXRESCHARS];
	float tx, ty, w, h;
	float hotx, hoty;
	uint32_t blend;
	uint32_t color;
	float z;
	bool bXFlip, bYFlip;
	//	float		x,y;
	//	float		scale;
	//	float		rotation;
	//	int			collision;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RAnimation: public RSprite
{
	int frames;
	float fps;
	anim_mode_t mode;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RFont: public ResDesc
{
	char filename[MAXRESCHARS];
	bool mipmap;
	uint32_t blend;
	uint32_t color;
	float z;
	float scale;
	float proportion;
	float tracking;
	float spacing;
	float rotation;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RParticle: public ResDesc
{
	char filename[MAXRESCHARS];
	char spritename[MAXRESCHARS];

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RDistort: public ResDesc
{
	char texname[MAXRESCHARS];
	float tx, ty, w, h;
	int cols, rows;
	uint32_t blend;
	uint32_t color;
	float z;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

struct RStringTable: public ResDesc
{
	char filename[MAXRESCHARS];

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name,
			const char *basename);
	virtual uint32_t Get(hgeResourceManager *rm);
	virtual void Free();
};

} // namespace hge
