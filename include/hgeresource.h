/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeResourceManager helper class header
 */

#pragma once

#include <hge.h>
#include <hgesprite.h>
#include <hgeanim.h>
#include <hgefont.h>
#include <hgeparticle.h>
#include <hgedistort.h>
#include <hgestrings.h>

namespace hge {

enum {
	RESTYPES = 13,
	MAXRESCHARS = 128
};


class hgeResourceManager;

struct HGE_EXPORT ResDesc
{
	char		name[MAXRESCHARS];
	int			resgroup;
	uint32_t	handle;
	ResDesc *	next;

	ResDesc();
	~ResDesc();

	virtual uint32_t Get(hgeResourceManager *rm) = 0;
	virtual void  Free() = 0;

protected:
	static HGE * get_hge();
	//static HGE	*g_resdesc_hge;
};

/*
** HGE Resource manager class
*/
class HGE_EXPORT hgeResourceManager
{
public:
	hgeResourceManager(const char *scriptname=0);
	~hgeResourceManager();

	void				ChangeScript(const char *scriptname=0);
	bool				Precache(int groupid=0);
	void				Purge(int groupid=0);

	void*				GetResource(const char *name, int resgroup=0);
	HTEXTURE			GetTexture(const char *name, int resgroup=0);
	HEFFECT				GetEffect(const char *name, int resgroup=0);
	HMUSIC				GetMusic(const char *name, int resgroup=0);
	HSTREAM				GetStream(const char *name, int resgroup=0);
	HTARGET				GetTarget(const char *name);

	hgeSprite*			GetSprite(const char *name);
	hgeAnimation*		GetAnimation(const char *name);
	hgeFont*			GetFont(const char *name);
	hgeParticleSystem*	GetParticleSystem(const char *name);
	hgeDistortionMesh*	GetDistortionMesh(const char *name);
	hgeStringTable*		GetStringTable(const char *name, int resgroup=0);

	// TODO: replace with STL container
	ResDesc*			m_resources[RESTYPES];

private:
	hgeResourceManager(const hgeResourceManager &);
	hgeResourceManager&	operator= (const hgeResourceManager&);
	void				_remove_all();
	void				_parse_script(const char *scriptname=0);

	//static HGE			* g_rmanager_hge;
	static HGE * get_hge();
};

} // namespace hge
