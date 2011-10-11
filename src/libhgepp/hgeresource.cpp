/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeResourceManager helper class implementation
 */

#include <hgeresource.h>
#include "parser.h"
#include "resources.h"

#include <algorithm>

namespace hge
{

	class HGE_Impl;
	extern HGE_Impl * g_hge_singleton;

//HGE * g_rmanager_hge = nullptr;
//HGE * g_resdesc_hge = nullptr;

hgeResourceManager::hgeResourceManager(const char *scriptname)
{
	//g_rmanager_hge = hgeCreate(HGE_VERSION);

// 	for (int i = 0; i < RESOURCE_TYPE_COUNT; i++)
// 		m_resources[i] = nullptr;
	_parse_script(scriptname);
}

hgeResourceManager::~hgeResourceManager()
{
	_remove_all();
	//g_rmanager_hge->Release();
}

void hgeResourceManager::_parse_script(const std::string & scriptname)
{
	//script_resource_t *rc, *rcnext;

	if (! scriptname.empty())
	{
		script_resource_t::Parse(this, NULL, scriptname, NULL);

// 		rc = m_res_scripts[RES_SCRIPT];
// 		while (rc)
// 		{
// 			rc->Free();
// 			rcnext = rc->next;
// 			delete rc;
// 			rc = rcnext;
// 		}
		m_res_scripts.clear();
//		m_resources[RES_SCRIPT] = nullptr;
	}
}

void hgeResourceManager::_remove_all()
{
	m_textures.clear();
	m_targets.clear();
	m_sprites.clear();
	m_animations.clear();
	m_fonts.clear();
	m_psystems.clear();
	m_distort_meshes.clear();
	m_string_tables.clear();
	m_bass_effects.clear();
	m_bass_musics.clear();
	m_bass_streams.clear();
	m_res_scripts.clear();
	m_bytes.clear();

// 	int i;
// 	BaseResource *rc, *rcnext;
// 
// 	for (i = 0; i < RESOURCE_TYPE_COUNT; i++)
// 	{
// 		rc = m_resources[i];
// 		while (rc)
// 		{
// 			rc->Free();
// 			rcnext = rc->next;
// 			delete rc;
// 			rc = rcnext;
// 		}
// 		m_resources[i] = nullptr;
// 	}
}

void hgeResourceManager::ChangeScript(const char *scriptname)
{
	_remove_all();
	_parse_script(scriptname);
}


bool hgeResourceManager::Precache(int groupid)
{
	// Say hello to missing closures, so we have to do it via globals
	bool result = true;

#define PRECACHE_FOREACH(MAPVAR,MAPTYPE) \
	std::for_each( MAPVAR.begin(), MAPVAR.end(), \
		[&result,groupid,this]( const MAPTYPE::pair_t & rc ) { \
			if (! groupid || groupid == rc.second->resgroup) result &= bool(rc.second->Get(this)); \
		})

	PRECACHE_FOREACH(m_textures, texture_resource_t);
	PRECACHE_FOREACH(m_targets, target_resource_t);
	PRECACHE_FOREACH(m_animations, animation_resource_t);
	PRECACHE_FOREACH(m_fonts, font_resource_t);
	PRECACHE_FOREACH(m_psystems, psystem_resource_t);
	PRECACHE_FOREACH(m_distort_meshes, distort_resource_t);
	PRECACHE_FOREACH(m_string_tables, string_table_resource_t);
	PRECACHE_FOREACH(m_bass_effects, bass_effect_resource_t);
	PRECACHE_FOREACH(m_bass_musics, bass_music_resource_t);
	PRECACHE_FOREACH(m_bass_streams, bass_stream_resource_t);
	PRECACHE_FOREACH(m_res_scripts, script_resource_t);
	PRECACHE_FOREACH(m_bytes, bytes_resource_t);

#undef PRECACHE_FOREACH

// 	m_targets.clear();
// 	m_sprites.clear();
// 	m_animations.clear();
// 	m_fonts.clear();
// 	m_psystems.clear();
// 	m_distorts.clear();
// 	m_string_tables.clear();
// 	m_effects.clear();
// 	m_musics.clear();
// 	m_streams.clear();
// 	m_res_scripts.clear();
// 	m_bytes.clear();

	return result;
}

void hgeResourceManager::Purge(int groupid)
{
#define PURGE_FOREACH(MAPVAR,MAPTYPE) \
	std::for_each( MAPVAR.begin(), MAPVAR.end(), \
		[groupid]( const MAPTYPE::pair_t & rc ) { \
			if (! groupid || groupid == rc.second->resgroup) rc.second->Free(); \
		})

	PURGE_FOREACH(m_textures, texture_resource_t);
	PURGE_FOREACH(m_targets, target_resource_t);
	PURGE_FOREACH(m_animations, animation_resource_t);
	PURGE_FOREACH(m_fonts, font_resource_t);
	PURGE_FOREACH(m_psystems, psystem_resource_t);
	PURGE_FOREACH(m_distort_meshes, distort_resource_t);
	PURGE_FOREACH(m_string_tables, string_table_resource_t);
	PURGE_FOREACH(m_bass_effects, bass_effect_resource_t);
	PURGE_FOREACH(m_bass_musics, bass_music_resource_t);
	PURGE_FOREACH(m_bass_streams, bass_stream_resource_t);
	PURGE_FOREACH(m_res_scripts, script_resource_t);
	PURGE_FOREACH(m_bytes, bytes_resource_t);

#undef PURGE_FOREACH
// 	int i;
// 	BaseResource *rc;
// 
// 	for (i = 0; i < RESOURCE_TYPE_COUNT; i++)
// 	{
// 		rc = m_resources[i];
// 		while (rc)
// 		{
// 			if (!groupid || groupid == rc->resgroup)
// 				rc->Free();
// 			rc = rc->next;
// 		}
// 	}
}

#if 0
handle_t hgeResourceManager::GetResource(const std::string & name, int resgroup)
{
	handle_t reshandle;
	bytes_resource_t *resource;
	BaseResource *Res = FindRes(this, RES_RESOURCE, name);

	if (Res)
		return Res->Get(this);
	else
	{
		reshandle = g_rmanager_hge->Resource_Load(name);
		if (reshandle)
		{
			resource = new bytes_resource_t();
			resource->handle = reshandle;
			resource->resgroup = resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_RESOURCE, resource);

			return reshandle;
		}
	}

	return nullptr;
}
#endif //0

HTEXTURE hgeResourceManager::GetTexture(const std::string & name, int resgroup)
{
	auto itr = m_textures.find( name );
	if( itr != m_textures.end() ) {
		return itr->second->handle;
	}
	else
	{
		auto res_handle = get_hge()->Texture_Load( name.c_str() );
		if( res_handle )
		{
			auto resource = std::shared_ptr<texture_resource_t>(new texture_resource_t());
			resource->handle = res_handle;
			resource->resgroup = resgroup;
			resource->mipmap = false;
			resource->filename = name;
			m_textures[name] = resource;
			
			return res_handle;
		}
	}
	return HTEXTURE();
/*
	HTEXTURE reshandle;
	RTexture *resource;
	BaseResource *Res = FindRes(this, RES_TEXTURE, name);
	if (Res)
		return (HTEXTURE) Res->Get(this);
	else
	{
		reshandle = g_rmanager_hge->Texture_Load(name);
		if (reshandle)
		{
			resource = new RTexture();
			resource->handle = reshandle;
			resource->resgroup = resgroup;
			resource->mipmap = false;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_TEXTURE, resource);

			return reshandle;
		}
	}
	return nullptr;
*/
}

HEFFECT hgeResourceManager::GetEffect(const std::string & name, int resgroup)
{
	auto itr = m_bass_effects.find( name );
	if( itr != m_bass_effects.end() ) {
		return itr->second->handle;
	}
	else
	{
		auto res_handle = get_hge()->Effect_Load( name.c_str() );
		if( res_handle )
		{
			auto resource = std::shared_ptr<bass_effect_resource_t>(new bass_effect_resource_t());
			resource->handle = res_handle;
			resource->resgroup = resgroup;
			resource->filename = name;
			m_bass_effects[name] = resource;
			
			return res_handle;
		}
	}
	return HEFFECT();
/*
	HEFFECT reshandle;
	REffect *resource;
	BaseResource *Res = FindRes(this, RES_EFFECT, name);
	if (Res)
		return (HEFFECT) Res->Get(this);
	else
	{
		reshandle = g_rmanager_hge->Effect_Load(name);
		if (reshandle)
		{
			resource = new REffect();
			resource->handle = reshandle;
			resource->resgroup = resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_EFFECT, resource);

			return reshandle;
		}
	}

	return nullptr;
*/
}

HMUSIC hgeResourceManager::GetMusic(const std::string & name, int resgroup)
{
	auto itr = m_bass_musics.find( name );
	if( itr != m_bass_musics.end() ) {
		return itr->second->handle;
	}
	else
	{
		HMUSIC res_handle = get_hge()->Music_Load( name.c_str() );
		if( res_handle )
		{
			auto resource = std::shared_ptr<bass_music_resource_t>(new bass_music_resource_t());
			resource->handle = res_handle;
			resource->resgroup = resgroup;
			resource->filename = name;
			m_bass_musics[name] = resource;
			
			return res_handle;
		}
	}
	return HMUSIC();
/*
	HMUSIC reshandle;
	RMusic *resource;
	BaseResource *Res = FindRes(this, RES_MUSIC, name);
	if (Res)
		return (HMUSIC) Res->Get(this);
	else
	{
		reshandle = g_rmanager_hge->Music_Load(name);
		if (reshandle)
		{
			resource = new RMusic();
			resource->handle = reshandle;
			resource->resgroup = resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_MUSIC, resource);

			return reshandle;
		}
	}

	return nullptr;
*/
}

HSTREAM hgeResourceManager::GetStream(const std::string & name, int resgroup)
{
	auto itr = m_bass_streams.find( name );
	if( itr != m_bass_streams.end() ) {
		return itr->second->handle;
	}
	else
	{
		auto res_handle = get_hge()->Stream_Load( name.c_str() );
		if( res_handle )
		{
			auto resource = std::shared_ptr<bass_stream_resource_t>(new bass_stream_resource_t());
			resource->handle = res_handle;
			resource->resgroup = resgroup;
			resource->filename = name;
			m_bass_streams[name] = resource;
			
			return res_handle;
		}
	}
	return HSTREAM();
/*
	HSTREAM reshandle;
	RStream *resource;
	BaseResource *Res = FindRes(this, RES_STREAM, name);
	if (Res)
		return (HSTREAM) Res->Get(this);
	else
	{
		reshandle = g_rmanager_hge->Stream_Load(name);
		if (reshandle)
		{
			resource = new RStream();
			resource->handle = reshandle;
			resource->resgroup = resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_STREAM, resource);

			return reshandle;
		}
	}

	return nullptr;
*/
}

HTARGET hgeResourceManager::GetTarget(const std::string & name)
{
	auto res = find_target(name);
	return res ? res->Get(this) : HTARGET();
}

hgeSprite* hgeResourceManager::GetSprite(const std::string & name)
{
	auto res = find_sprite(name);
	return res ? res->Get(this) : nullptr;
}

hgeAnimation* hgeResourceManager::GetAnimation(const std::string & name)
{
	auto res = find_animation(name);
	return res ? res->Get(this) : nullptr;
}

hgeFont* hgeResourceManager::GetFont(const std::string & name)
{
	auto res = find_font(name);
	return res ? res->Get(this) : nullptr;
}

hgeParticleSystem* hgeResourceManager::GetParticleSystem(const std::string & name)
{
	auto res = find_particle_system(name);
	return res ? res->Get(this) : nullptr;
}

hgeDistortionMesh* hgeResourceManager::GetDistortionMesh(const std::string & name)
{
	auto res = find_distortion_mesh(name);
	return res ? res->Get(this) : nullptr;
}

hgeStringTable* hgeResourceManager::GetStringTable(const std::string & name, int resgroup)
{
	auto itr = m_string_tables.find( name );
	if( itr != m_string_tables.end() ) {
		return itr->second->handle;
	}
	else
	{
		auto strtable = new hgeStringTable(name);
		if( strtable )
		{
			auto resource = std::shared_ptr<string_table_resource_t>(new string_table_resource_t());
			resource->handle = strtable;
			resource->resgroup = resgroup;
			resource->filename = name;
			m_string_tables[name] = resource;
			
			return strtable;
		}
	}
	return nullptr;
/*
	hgeStringTable *strtable;
	RStringTable *resource;
	BaseResource *Res = FindRes(this, RES_STRTABLE, name);
	if (Res)
		return (hgeStringTable*) Res->Get(this).ptr;
	else
	{
		strtable = new hgeStringTable(name);
		if (strtable)
		{
			resource = new RStringTable();
			resource->handle = (uint32_t) strtable;
			resource->resgroup = resgroup;
			strcpy(resource->name, name);
			strcpy(resource->filename, name);
			AddRes(this, RES_STRTABLE, resource);

			return strtable;
		}
	}

	return nullptr;
*/
}

HGE * hgeResourceManager::get_hge()
{
	return (HGE*)g_hge_singleton;
}

} // namespace hge
