/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeResourceManager helper class implementation
 */


#include <hgeresource.h>
#include "parser.h"
#include "resources.h"


namespace hge {

HGE * g_rmanager_hge = nullptr;
HGE * g_resdesc_hge = nullptr;


hgeResourceManager::hgeResourceManager(const char *scriptname)
{
	g_rmanager_hge = hgeCreate(HGE_VERSION);

	for(int i=0;i<RESTYPES;i++) m_resources[i] = nullptr;
	_parse_script(scriptname);
}

hgeResourceManager::~hgeResourceManager()
{
	_remove_all();
	g_rmanager_hge->Release();
}

void hgeResourceManager::_parse_script(const char *scriptname)
{
	ResDesc *rc, *rcnext;

	if(scriptname)
	{
		RScript::Parse(this, NULL, scriptname, NULL);
		
		rc=m_resources[RES_SCRIPT];
		while(rc)
		{
			rc->Free();
			rcnext=rc->next;
			delete rc;
			rc=rcnext;
		}
		m_resources[RES_SCRIPT] = nullptr;
	}
}

void hgeResourceManager::_remove_all()
{
	int i;
	ResDesc *rc, *rcnext;

	for(i=0;i<RESTYPES;i++)
	{
		rc=m_resources[i];
		while(rc)
		{
			rc->Free();
			rcnext=rc->next;
			delete rc;
			rc=rcnext;
		}
		m_resources[i] = nullptr;
	}
}

void hgeResourceManager::ChangeScript(const char *scriptname)
{
	_remove_all();
	_parse_script(scriptname);
}

bool hgeResourceManager::Precache(int groupid)
{
	int i;
	ResDesc *rc;
	bool bResult=true;

	for(i=0;i<RESTYPES;i++)
	{
		rc=m_resources[i];
		while(rc)
		{
			if(!groupid || groupid==rc->resgroup) bResult=bResult && (rc->Get(this)!=0);
			rc=rc->next;
		}
	}

	return bResult;
}

void hgeResourceManager::Purge(int groupid)
{
	int i;
	ResDesc *rc;

	for(i=0;i<RESTYPES;i++)
	{
		rc=m_resources[i];
		while(rc)
		{
			if(!groupid || groupid==rc->resgroup) rc->Free();
			rc=rc->next;
		}
	}
}

void* hgeResourceManager::GetResource(const char *name, int resgroup)
{
	void *reshandle;
	RResource *resource;
	ResDesc *Res=FindRes(this, RES_RESOURCE, name);

	if(Res) return (void *)Res->Get(this);
	else
	{
		reshandle=g_rmanager_hge->Resource_Load(name);
		if(reshandle)
		{
			resource=new RResource();
			resource->handle=(uint32_t)reshandle;
			resource->resgroup=resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_RESOURCE, resource);
			
			return reshandle;
		}
	}

	return nullptr;
}

HTEXTURE hgeResourceManager::GetTexture(const char *name, int resgroup)
{
	HTEXTURE reshandle;
	RTexture *resource;
	ResDesc *Res=FindRes(this, RES_TEXTURE, name);
	if(Res) return (HTEXTURE)Res->Get(this);
	else
	{
		reshandle=g_rmanager_hge->Texture_Load(name);
		if(reshandle)
		{
			resource=new RTexture();
			resource->handle=reshandle;
			resource->resgroup=resgroup;
			resource->mipmap=false;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_TEXTURE, resource);

			return reshandle;
		}
	}

	return nullptr;
}

HEFFECT hgeResourceManager::GetEffect(const char *name, int resgroup)
{
	HEFFECT reshandle;
	REffect *resource;
	ResDesc *Res=FindRes(this, RES_EFFECT, name);
	if(Res) return (HEFFECT)Res->Get(this);
	else
	{
		reshandle=g_rmanager_hge->Effect_Load(name);
		if(reshandle)
		{
			resource=new REffect();
			resource->handle=reshandle;
			resource->resgroup=resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_EFFECT, resource);

			return reshandle;
		}
	}

	return nullptr;
}

HMUSIC hgeResourceManager::GetMusic(const char *name, int resgroup)
{
	HMUSIC reshandle;
	RMusic *resource;
	ResDesc *Res=FindRes(this, RES_MUSIC, name);
	if(Res) return (HMUSIC)Res->Get(this);
	else
	{
		reshandle=g_rmanager_hge->Music_Load(name);
		if(reshandle)
		{
			resource=new RMusic();
			resource->handle=reshandle;
			resource->resgroup=resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_MUSIC, resource);

			return reshandle;
		}
	}

	return nullptr;
}

HSTREAM hgeResourceManager::GetStream(const char *name, int resgroup)
{
	HSTREAM reshandle;
	RStream *resource;
	ResDesc *Res=FindRes(this, RES_STREAM, name);
	if(Res) return (HSTREAM)Res->Get(this);
	else
	{
		reshandle=g_rmanager_hge->Stream_Load(name);
		if(reshandle)
		{
			resource=new RStream();
			resource->handle=reshandle;
			resource->resgroup=resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_STREAM, resource);

			return reshandle;
		}
	}

	return nullptr;
}

HTARGET hgeResourceManager::GetTarget(const char *name)
{
	ResDesc *Res=FindRes(this, RES_TARGET, name);
	if(Res) return (HTARGET)Res->Get(this);
	else return nullptr;
}

hgeSprite* hgeResourceManager::GetSprite(const char *name)
{
	ResDesc *Res=FindRes(this, RES_SPRITE, name);
	if(Res) return (hgeSprite *)Res->Get(this);
	else return nullptr;
}

hgeAnimation* hgeResourceManager::GetAnimation(const char *name)
{
	ResDesc *Res=FindRes(this, RES_ANIMATION, name);
	if(Res) return (hgeAnimation *)Res->Get(this);
	else return nullptr;
}

hgeFont* hgeResourceManager::GetFont(const char *name)
{
	ResDesc *Res=FindRes(this, RES_FONT, name);
	if(Res) return (hgeFont *)Res->Get(this);
	else return nullptr;
}

hgeParticleSystem* hgeResourceManager::GetParticleSystem(const char *name)
{
	ResDesc *Res=FindRes(this, RES_PARTICLE, name);
	if(Res) return (hgeParticleSystem *)Res->Get(this);
	else return nullptr;
}

hgeDistortionMesh* hgeResourceManager::GetDistortionMesh(const char *name)
{
	ResDesc *Res=FindRes(this, RES_DISTORT, name);
	if(Res) return (hgeDistortionMesh *)Res->Get(this);
	else return nullptr;
}

hgeStringTable* hgeResourceManager::GetStringTable(const char *name, int resgroup)
{
	hgeStringTable *strtable;
	RStringTable *resource;
	ResDesc *Res=FindRes(this, RES_STRTABLE, name);
	if(Res) return (hgeStringTable*)Res->Get(this);
	else
	{
		strtable=new hgeStringTable(name);
		if(strtable)
		{
			resource=new RStringTable();
			resource->handle=(uint32_t)strtable;
			resource->resgroup=resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_STRTABLE, resource);

			return strtable;
		}
	}

	return nullptr;
}

HGE * hgeResourceManager::get_hge()
{
	return g_rmanager_hge;
}

ResDesc::ResDesc()
{
	g_resdesc_hge=hgeCreate(HGE_VERSION);
}

ResDesc::~ResDesc()
{
	g_resdesc_hge->Release();
}

HGE * ResDesc::get_hge()
{
	return g_resdesc_hge;
}

} // namespace hge
