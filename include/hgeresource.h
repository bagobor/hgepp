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

#include <string>
#include <map>


namespace hge {

enum
{
	RESOURCE_TYPE_COUNT = 13, MAXRESCHARS = 128
};


class hgeResourceManager;

template <class res_type_t>
class BaseResource
{
public:
	//std::string	name; - is stored in map keys
	int			resgroup;
	res_type_t	handle;
	//ResDesc *	next;

// 	typedef std::shared_ptr <BaseResource<res_type_t> > base_ptr_t;
// 	typedef std::map <std::string, ptr_t > map_t;
// 	typedef std::pair <std::string, ptr_t > pair_t;

	BaseResource() {}
	virtual ~BaseResource() {}

	virtual res_type_t Get(hgeResourceManager *rm) = 0;
	virtual void Free() = 0;

//protected:
	//static HGE * get_hge();
	//static HGE	*g_resdesc_hge;
};

// typedef BaseResource<HTARGET> target_resource_t;
// typedef BaseResource<HTEXTURE> texture_resource_t;
// typedef BaseResource<hgeSprite *> sprite_resource_t;
// typedef BaseResource<hgeAnimation *> animation_resource_t;
// typedef BaseResource<hgeFont *> font_resource_t;
// typedef BaseResource<hgeParticleSystem *> psystem_resource_t;
// typedef BaseResource<hgeDistortionMesh *> distort_resource_t;
// typedef BaseResource<hgeStringTable *> string_table_resource_t;
// typedef BaseResource<HEFFECT> bass_effect_resource_t;
// typedef BaseResource<HMUSIC> bass_music_resource_t;
// typedef BaseResource<HSTREAM> bass_stream_resource_t;

class RScriptParser;

class script_resource_t: public BaseResource<void *>
{
public:
	typedef std::shared_ptr <script_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual void * Get(hgeResourceManager *rm)
	{
		return nullptr;
	}
	virtual void Free()
	{
	}
};

class bytes_resource_t: public BaseResource<bytes_t>
{
public:
	typedef std::shared_ptr <bytes_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual bytes_t Get(hgeResourceManager *rm);
	virtual void Free();
};

class texture_resource_t: public BaseResource<HTEXTURE>
{
public:
	typedef std::shared_ptr <texture_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;
	bool mipmap;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual HTEXTURE Get(hgeResourceManager *rm);
	virtual void Free();
};

class bass_effect_resource_t: public BaseResource<HEFFECT>
{
public:
	typedef std::shared_ptr <bass_effect_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual HEFFECT Get(hgeResourceManager *rm);
	virtual void Free();
};

class bass_music_resource_t: public BaseResource<HMUSIC>
{
public:
	typedef std::shared_ptr <bass_music_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;
	int amplify;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual HMUSIC Get(hgeResourceManager *rm);
	virtual void Free();
};

class bass_stream_resource_t: public BaseResource<HSTREAM>
{
public:
	typedef std::shared_ptr <bass_stream_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual HSTREAM Get(hgeResourceManager *rm);
	virtual void Free();
};

class target_resource_t: public BaseResource<HTARGET>
{
public:
	typedef std::shared_ptr <target_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	int width;
	int height;
	bool zbuffer;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual HTARGET Get(hgeResourceManager *rm);
	virtual void Free();
};

class sprite_resource_t: public BaseResource<hgeSprite *>
{
public:
	typedef std::shared_ptr <sprite_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string texname;
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

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual hgeSprite * Get(hgeResourceManager *rm);
	virtual void Free();
};

class animation_resource_t: public sprite_resource_t
{
public:
	typedef std::shared_ptr <animation_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	int frames;
	float fps;
	anim_mode_t mode;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual hgeAnimation * Get(hgeResourceManager *rm);
	virtual void Free();
};

class font_resource_t: public BaseResource<hgeFont *>
{
public:
	typedef std::shared_ptr <font_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;
	bool mipmap;
	uint32_t blend;
	uint32_t color;
	float z;
	float scale;
	float proportion;
	float tracking;
	float spacing;
	float rotation;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual hgeFont * Get(hgeResourceManager *rm);
	virtual void Free();
};

class psystem_resource_t: public BaseResource<hgeParticleSystem *>
{
public:
	typedef std::shared_ptr <psystem_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;
	std::string spritename;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual hgeParticleSystem * Get(hgeResourceManager *rm);
	virtual void Free();
};

class distort_resource_t: public BaseResource<hgeDistortionMesh *>
{
public:
	typedef std::shared_ptr <distort_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string texname;
	float tx, ty, w, h;
	int cols, rows;
	uint32_t blend;
	uint32_t color;
	float z;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual hgeDistortionMesh * Get(hgeResourceManager *rm);
	virtual void Free();
};

struct string_table_resource_t: public BaseResource<hgeStringTable *>
{
public:
	typedef std::shared_ptr <string_table_resource_t> ptr_t;
	typedef std::map <std::string, ptr_t > map_t;
	typedef std::pair <std::string, ptr_t > pair_t;

	std::string filename;

	static void Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
			const std::string & basename);
	virtual hgeStringTable * Get(hgeResourceManager *rm);
	virtual void Free();
};

/*
** HGE Resource manager class
*/
class HGE_EXPORT hgeResourceManager
{
public:
	hgeResourceManager(const char *scriptname = 0);
	~hgeResourceManager();

	void ChangeScript(const char *scriptname = 0);
	bool Precache(int groupid = 0);
	void Purge(int groupid = 0);

	//handle_t GetResource(const std::string & name, int resgroup = 0);
	HTEXTURE GetTexture(const std::string & name, int resgroup = 0);
	HEFFECT GetEffect(const std::string & name, int resgroup = 0);
	HMUSIC GetMusic(const std::string & name, int resgroup = 0);
	HSTREAM GetStream(const std::string & name, int resgroup = 0);
	HTARGET GetTarget(const std::string & name);

	hgeSprite* GetSprite(const std::string & name);
	hgeAnimation* GetAnimation(const std::string & name);
	hgeFont* GetFont(const std::string & name);
	hgeParticleSystem* GetParticleSystem(const std::string & name);
	hgeDistortionMesh* GetDistortionMesh(const std::string & name);
	hgeStringTable* GetStringTable(const std::string & name, int resgroup = 0);

	texture_resource_t::ptr_t find_texture( const std::string & name ) {
		auto itr = m_textures.find( name );
		return ( itr == m_textures.end() ) ? nullptr : itr->second;
	}
	target_resource_t::ptr_t find_target( const std::string & name ) {
		auto itr = m_targets.find( name );
		return ( itr == m_targets.end() ) ? nullptr : itr->second;
	}
	sprite_resource_t::ptr_t find_sprite( const std::string & name ) {
		auto itr = m_sprites.find( name );
		return ( itr == m_sprites.end() ) ? nullptr : itr->second;
	}
	animation_resource_t::ptr_t find_animation( const std::string & name ) {
		auto itr = m_animations.find( name );
		return ( itr == m_animations.end() ) ? nullptr : itr->second;
	}
	font_resource_t::ptr_t find_font( const std::string & name ) {
		auto itr = m_fonts.find( name );
		return ( itr == m_fonts.end() ) ? nullptr : itr->second;
	}
	psystem_resource_t::ptr_t find_particle_system( const std::string & name ) {
		auto itr = m_psystems.find( name );
		return ( itr == m_psystems.end() ) ? nullptr : itr->second;
	}
	distort_resource_t::ptr_t find_distortion_mesh( const std::string & name ) {
		auto itr = m_distorts.find( name );
		return ( itr == m_distorts.end() ) ? nullptr : itr->second;
	}
	string_table_resource_t::ptr_t find_string_table( const std::string & name ) {
		auto itr = m_string_tables.find( name );
		return ( itr == m_string_tables.end() ) ? nullptr : itr->second;
	}
	bass_effect_resource_t::ptr_t find_bass_effect( const std::string & name ) {
		auto itr = m_effects.find( name );
		return ( itr == m_effects.end() ) ? nullptr : itr->second;
	}
	bass_music_resource_t::ptr_t find_bass_music( const std::string & name ) {
		auto itr = m_musics.find( name );
		return ( itr == m_musics.end() ) ? nullptr : itr->second;
	}
	bass_stream_resource_t::ptr_t find_bass_stream( const std::string & name ) {
		auto itr = m_streams.find( name );
		return ( itr == m_streams.end() ) ? nullptr : itr->second;
	}
	script_resource_t::ptr_t find_script( const std::string & name ) {
		auto itr = m_res_scripts.find( name );
		return ( itr == m_res_scripts.end() ) ? nullptr : itr->second;
	}
	bytes_resource_t::ptr_t find_bytes( const std::string & name ) {
		auto itr = m_bytes.find( name );
		return ( itr == m_bytes.end() ) ? nullptr : itr->second;
	}

	texture_resource_t::map_t		m_textures;
	target_resource_t::map_t		m_targets;
	sprite_resource_t::map_t		m_sprites;
	animation_resource_t::map_t		m_animations;
	font_resource_t::map_t			m_fonts;
	psystem_resource_t::map_t		m_psystems;
	distort_resource_t::map_t		m_distorts;
	string_table_resource_t::map_t	m_string_tables;
	bass_effect_resource_t::map_t	m_effects;
	bass_music_resource_t::map_t	m_musics;
	bass_stream_resource_t::map_t	m_streams;
	script_resource_t::map_t		m_res_scripts;
	bytes_resource_t::map_t			m_bytes;

	static HGE * get_hge();

private:
	hgeResourceManager(const hgeResourceManager &);
	hgeResourceManager& operator=(const hgeResourceManager&);
	void _remove_all();
	void _parse_script(const std::string & scriptname = 0);

	//static HGE			* g_rmanager_hge;
};

} // namespace hge
