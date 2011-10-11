/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Resource script parser header
 */
#pragma once

#include <hgepp.h>

#include <string.h>
#include <string>
#include <stdlib.h>

namespace hge
{

enum
{
 TTNONE, TTEND, TTNUMBER, TTSTRING,

 TTBOOL, TTEQUALS, TTBASED, TTSEPARATOR, TTOPENBLOCK, TTCLOSEBLOCK,
 
 TTRES__FIRST,
 TTRES_INCLUDE, TTRES_RESOURCE, TTRES_TEXTURE, TTRES_SOUND, TTRES_MUSIC,
 TTRES_STREAM, TTRES_TARGET, TTRES_SPRITE, TTRES_ANIMATION, TTRES_FONT,
 TTRES_PARTICLE, TTRES_DISTORT, TTRES_STRTABLE,
 TTRES__LAST,

 TTPAR__FIRST,
 TTPAR_FILENAME, TTPAR_RESGROUP, TTPAR_MIPMAP, TTPAR_AMPLIFY, TTPAR_SIZE, TTPAR_ZBUFFER,
 TTPAR_TEXTURE, TTPAR_RECT, TTPAR_HOTSPOT, TTPAR_BLENDMODE, TTPAR_COLOR,
 TTPAR_ZORDER, TTPAR_FLIP, TTPAR_SCALE, TTPAR_PROPORTION, TTPAR_ROTATION, TTPAR_FRAMES,
 TTPAR_FPS, TTPAR_MODE, TTPAR_TRACKING, TTPAR_SPACING, TTPAR_SPRITE, TTPAR_MESH,
 TTPAR__LAST,

 TTCON__FIRST,
 TTCON_COLORMUL, TTCON_COLORADD, TTCON_ALPHABLND, TTCON_ALPHAADD, TTCON_ZWRITE,
 TTCON_NOZWRITE, TTCON_FORWARD, TTCON_REVERSE, TTCON_PINGPONG, TTCON_NOPINGPONG,
 TTCON_LOOP, TTCON_NOLOOP, TTCON_CIRCLE, TTCON_RECT, TTCON_ALPHA,
 TTCON__LAST
};


class RScriptParser
{
public:
	RScriptParser(const char *name, const char *scr);
	~RScriptParser();

	int get_token();
	void put_back();
	int get_line();
	const std::string & get_name();

	char * token_as_cstring();
	int token_as_int();
	float token_as_float();
	bool token_as_bool();
	uint32_t token_as_hex();

	void ScriptPostError(char *msg1, char *msg2);

	int tokentype;
	char tokenvalue[128];
	const char * script;
	std::string scriptname;
	int line;

private:
	bool strtkcmp(const char *str, const char *mem);

	//static HGE *g_scriptparser_hge;
};

} // namespace hge