/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: ini file
 */
#include "hge_impl.h"

namespace hge {

void HGE_CALL HGE_Impl::Ini_SetInt(const char *section, const char *name, int value)
{
	char buf[256];

	if(m_ini_file[0]) {
		sprintf(buf,"%d",value);
		WritePrivateProfileString(section, name, buf, m_ini_file);
	}
}


int HGE_CALL HGE_Impl::Ini_GetInt(const char *section, const char *name, int def_val)
{
	char buf[256];

	if(m_ini_file[0]) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), m_ini_file))
		{ return atoi(buf); }
		else { return def_val; }
	}
	return def_val;
}


void HGE_CALL HGE_Impl::Ini_SetFloat(const char *section, const char *name, float value)
{
	char buf[256];

	if(m_ini_file[0]) {
		sprintf(buf,"%f",value);
		WritePrivateProfileString(section, name, buf, m_ini_file);
	}
}


float HGE_CALL HGE_Impl::Ini_GetFloat(const char *section, const char *name, float def_val)
{
	char buf[256];

	if(m_ini_file[0]) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), m_ini_file))
		{ return (float)atof(buf); }
		else { return def_val; }
	}
	return def_val;
}


void HGE_CALL HGE_Impl::Ini_SetString(const char *section, const char *name, const char *value)
{
	if(m_ini_file[0]) WritePrivateProfileString(section, name, value, m_ini_file);
}


char* HGE_CALL HGE_Impl::Ini_GetString(const char *section, const char *name, const char *def_val)
{
	if(m_ini_file[0]) GetPrivateProfileString(section, name, def_val, m_ini_string, sizeof(m_ini_string), m_ini_file);
	else strcpy(m_ini_string, def_val);
	return m_ini_string;
}

} // namespace hge
