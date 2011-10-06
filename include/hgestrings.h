/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeStringTable helper class header
 */
#pragma once

#include <hge.h>

namespace hge {

#define MAXSTRNAMELENGTH 64


struct NamedString
{
	char name[MAXSTRNAMELENGTH];
	char *string;
	NamedString *next;
};

/*
** HGE String table class
*/
class HGE_EXPORT hgeStringTable
{
public:
	hgeStringTable(const char *filename);
	~hgeStringTable();

	char *GetString(const char *name);

private:
	hgeStringTable(const hgeStringTable &);
	hgeStringTable& operator=(const hgeStringTable &);

	NamedString *strings;

	//static HGE		*g_hgestringtab_hge;
	static HGE * get_hge();
};

} // namespace hge
