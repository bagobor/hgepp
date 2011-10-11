/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeResourceManager resources implementation
 */
#include <hgepp/res_manager.h>
#include <hgepp/res_script.h>
//#include "resources.h"

namespace hge
{

/////////////// COMMON //

/*
void AddRes(hgeResourceManager *rm, int type, BaseResource *resource)
{
	resource->next = rm->m_resources[type];
	rm->m_resources[type] = resource;
}
*/

/*
BaseResource *FindRes(hgeResourceManager *rm, int type, const char *name)
{
	BaseResource *rc;

	rc = rm->m_resources[type];
	while (rc)
	{
		if (!strcmp(name, rc->name))
			return rc;
		rc = rc->next;
	}

	return 0;
}
*/

bool ScriptSkipToNextParameter(RScriptParser *sp, bool bIgnore)
{
	bool bToBeIgnored = bIgnore;
	if (bIgnore)
		sp->put_back();

	for (;;)
	{
		sp->get_token();
		if (sp->tokentype == TTCLOSEBLOCK)
		{
			if (bIgnore)
			{
				sp->put_back();
				return true;
			}
			return false;
		}
		if ((sp->tokentype > TTRES__FIRST && sp->tokentype < TTRES__LAST) || sp->tokentype == TTEND)
		{
			sp->put_back();
			if (bIgnore)
				return true;
			sp->ScriptPostError("'}' missed, ", " encountered.");
			return false;
		}
		if ((sp->tokentype <= TTPAR__FIRST && sp->tokentype >= TTPAR__LAST) || bToBeIgnored)
		{
			bToBeIgnored = false;
			sp->ScriptPostError("Unsupported resource parameter ", ".");
			do
				sp->get_token();
			while ((sp->tokentype <= TTPAR__FIRST || sp->tokentype >= TTPAR__LAST)
					&& (sp->tokentype <= TTRES__FIRST || sp->tokentype >= TTRES__LAST)
					&& sp->tokentype != TTCLOSEBLOCK && sp->tokentype != TTEND);
			sp->put_back();
		}
		else
		{
			if (bIgnore)
				sp->put_back();
			return true;
		}
	}
}

template <class RTYPE>
void ScriptParseFileResource(hgeResourceManager *res_manager, RScriptParser *sp,
	const std::string & name, typename RTYPE::map_t & store_into, typename RTYPE::ptr_t base )
{
	//bytes_resource_t *rc = (bytes_resource_t *) rr;
	auto rc = RTYPE::ptr_t( new RTYPE() );
	//auto base = res_manager->find_bytes( basename );

	//base = (bytes_resource_t *) FindRes(rm, restype, basename);
	if (base)
	{
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->filename.clear();
	}
	rc->handle = RTYPE::handle_type_t();
	//rc->name = name;

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_FILENAME:
			sp->get_token();
			sp->get_token();
			rc->filename = sp->token_as_cstring();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	//AddRes(rm, restype, rc);
	//res_manager->m_bytes[name] = rc;
	store_into[name] = rc;
}

void ScriptParseBlendMode(RScriptParser *sp, uint32_t *blend)
{
	for (;;)
	{
		sp->get_token();
		if (sp->tokentype != TTEQUALS && sp->tokentype != TTSEPARATOR)
		{
			sp->put_back();
			return;
		}

		switch (sp->get_token())
		{
		case TTCON_COLORMUL:
			*blend &= ~BLEND_COLORADD;
			break;
		case TTCON_COLORADD:
			*blend |= BLEND_COLORADD;
			break;
		case TTCON_ALPHABLND:
			*blend |= BLEND_ALPHABLEND;
			break;
		case TTCON_ALPHAADD:
			*blend &= ~BLEND_ALPHABLEND;
			break;
		case TTCON_ZWRITE:
			*blend |= BLEND_ZWRITE;
			break;
		case TTCON_NOZWRITE:
			*blend &= ~BLEND_ZWRITE;
			break;
		default:
			sp->ScriptPostError("Unsupported value ", ".");
			break;
		}
	}
}

void ScriptParseSpriteAnim(RScriptParser *sp, sprite_resource_t *rc, bool anim)
{
	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_TEXTURE:
			sp->get_token();
			sp->get_token();
			rc->texname = sp->token_as_cstring();
			break;

		case TTPAR_RECT:
			sp->get_token();
			sp->get_token();
			rc->tx = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->ty = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->w = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->h = sp->token_as_float();
			break;

		case TTPAR_HOTSPOT:
			sp->get_token();
			sp->get_token();
			rc->hotx = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->hoty = sp->token_as_float();
			break;

		case TTPAR_BLENDMODE:
			ScriptParseBlendMode(sp, &rc->blend);
			break;

		case TTPAR_COLOR:
			sp->get_token();
			sp->get_token();
			rc->color = sp->token_as_hex();
			break;

		case TTPAR_ZORDER:
			sp->get_token();
			sp->get_token();
			rc->z = sp->token_as_float();
			break;

		case TTPAR_FLIP:
			sp->get_token();
			sp->get_token();
			rc->bXFlip = sp->token_as_bool();
			sp->get_token();
			sp->get_token();
			rc->bYFlip = sp->token_as_bool();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		case TTPAR_FRAMES:
			if (anim)
			{
				sp->get_token();
				sp->get_token();
				auto rca = dynamic_cast <animation_resource_t *> (rc);
				_ASSERTE( rca && "if this crashes means you attempted to load FRAMES for non-animation resource" );
				if( rca ) rca->frames = sp->token_as_int();
				break;
			}

		case TTPAR_FPS:
			if (anim)
			{
				sp->get_token();
				sp->get_token();
				auto rca = dynamic_cast <animation_resource_t *> (rc);
				_ASSERTE( rca && "if this crashes means you attempted to load FPS for non-animation resource" );
				if( rca ) rca->fps = sp->token_as_float();
				break;
			}

		case TTPAR_MODE:
			if (anim)
			{
				for (;;)
				{
					sp->get_token();
					if (sp->tokentype != TTEQUALS && sp->tokentype != TTSEPARATOR)
					{
						sp->put_back();
						break;
					}

					switch (sp->get_token())
					{
					case TTCON_FORWARD: {
						auto rca = dynamic_cast <animation_resource_t *> (rc);
						_ASSERTE( rca && "if this crashes means you attempted to use FORWARD for non-animation resource" );
						if( rca ) rca->mode = (anim_mode_t) (rca->mode & ~HGEANIM_REV);
										} break;

					case TTCON_REVERSE: {
						auto rca = dynamic_cast <animation_resource_t *> (rc);
						_ASSERTE( rca && "if this crashes means you attempted to use REVERSE for non-animation resource" );
						if( rca ) rca->mode = (anim_mode_t) (rca->mode | HGEANIM_REV);
										} break;

					case TTCON_NOPINGPONG: {
						auto rca = dynamic_cast <animation_resource_t *> (rc);
						_ASSERTE( rca && "if this crashes means you attempted to use NOPINGPONG for non-animation resource" );
						if( rca ) rca->mode = (anim_mode_t) (rca->mode & ~HGEANIM_PINGPONG);
										   } break;

					case TTCON_PINGPONG: {
						auto rca = dynamic_cast <animation_resource_t *> (rc);
						_ASSERTE( rca && "if this crashes means you attempted to use PINGPONG for non-animation resource" );
						if( rca ) rca->mode = (anim_mode_t) (rca->mode | HGEANIM_PINGPONG);
										 } break;

					case TTCON_NOLOOP: {
						auto rca = dynamic_cast <animation_resource_t *> (rc);
						_ASSERTE( rca && "if this crashes means you attempted to use NOLOOP for non-animation resource" );
						if( rca ) rca->mode = (anim_mode_t) (rca->mode & ~HGEANIM_LOOP);
									   } break;

					case TTCON_LOOP: {
						auto rca = dynamic_cast <animation_resource_t *> (rc);
						_ASSERTE( rca && "if this crashes means you attempted to use LOOP for non-animation resource" );
						if( rca ) rca->mode = (anim_mode_t) (rca->mode | HGEANIM_LOOP);
									 } break;

					default:
						sp->ScriptPostError("Unsupported value ", ".");
						break;
					}
				}
				break;
			}

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}
}

/////////////// RScript //

void script_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & sname,
		const std::string & sbasename)
{
	uint32_t size;
	std::string name;
	std::string basename;
	int restype;

	if (! rm->find_script(sname))
	{
		// hack! we need an instance of RScript to access hge
		auto res_script = script_resource_t::ptr_t(new script_resource_t());
		// if all ok, this object is used later to keep the script

		auto data = hgeResourceManager::get_hge()->Resource_Load(sname.c_str(), &size);
		if (!data)
		{
			if (sp)
				sp->ScriptPostError("Script ", " not found.");
			else
				hgeResourceManager::get_hge()->System_Log("Script '%s' not found.", sname);
			//delete res_script;
			return;
		}
		else
		{
			std::unique_ptr<char> script(new char[size + 1]);
			memcpy(script.get(), data.get(), size);
			script.get()[size] = 0;
			//get_hge()->Resource_Free(data);
			data.reset();

			//strcpy(res_script->name, sname);
			//AddRes(rm, RES_SCRIPT, res_script);
			rm->m_res_scripts[sname] = res_script;

			std::unique_ptr<RScriptParser> np( new RScriptParser(sname.c_str(), script.get()) );

			for (;;)
			{
				np->get_token();
				if (np->tokentype == TTEND)
					break;

				else if (np->tokentype == TTRES_INCLUDE)
				{
					np->get_token();
					script_resource_t::Parse(rm, np.get(), np->token_as_cstring(), nullptr);
				}

				else if (np->tokentype > TTRES__FIRST && np->tokentype < TTRES__LAST)
				{
					restype = np->tokentype - TTRES__FIRST - 1;
					name.clear();
					basename.clear();

					np->get_token();
					// moved existence test inside type-specific routines
					/*
					if (FindRes(rm, restype, np->token_as_cstring()))
					{
						np->ScriptPostError("Resource ",
								" of the same type already has been defined.");
						while ((np->tokentype <= TTRES__FIRST || np->tokentype >= TTRES__LAST)
								&& np->tokentype != TTEND)
							np->get_token();
						np->put_back();
						continue;
					}
					*/
					name = np->token_as_cstring();

					np->get_token();

					if (np->tokentype == TTBASED)
					{
						np->get_token();
						// moved existence test inside type-specific routines
						/*
						if (!FindRes(rm, restype, np->token_as_cstring()))
							np->ScriptPostError("Base resource ", " is not defined.");
						else
						*/
							basename = np->token_as_cstring();
						np->get_token();
					}

					if (np->tokentype == TTOPENBLOCK)
					{
						switch (restype)
						{
						case RES_RESOURCE:
							bytes_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_TEXTURE:
							texture_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_EFFECT:
							bass_effect_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_MUSIC:
							bass_music_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_STREAM:
							bass_stream_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_TARGET:
							target_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_SPRITE:
							sprite_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_ANIMATION:
							animation_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_FONT:
							font_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_PARTICLE:
							psystem_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_DISTORT:
							distort_resource_t::Parse(rm, np.get(), name, basename);
							break;
						case RES_STRTABLE:
							string_table_resource_t::Parse(rm, np.get(), name, basename);
							break;
						}
					}
					else
					{
						np->ScriptPostError("Illegal resource syntax, ", " found; '{' expected.");
						while ((np->tokentype <= TTRES__FIRST || np->tokentype >= TTRES__LAST)
								&& np->tokentype != TTEND)
							np->get_token();
						np->put_back();
					}
				}

				else
				{
					np->ScriptPostError("Unrecognized resource specificator ", ".");
					while ((np->tokentype <= TTRES__FIRST || np->tokentype >= TTRES__LAST)
							&& np->tokentype != TTEND)
						np->get_token();
					np->put_back();
				}
			}

			//delete np;
			//delete[] script;
		}
	}
	else
		sp->ScriptPostError("Script ", " already has been parsed.");
}

/////////////// RResource //

void bytes_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	ScriptParseFileResource <bytes_resource_t> (rm, sp, name, rm->m_bytes, rm->find_bytes(basename) );
}

bytes_t bytes_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
		handle = hgeResourceManager::get_hge()->Resource_Load(filename.c_str());
	return handle;
}

void bytes_resource_t::Free()
{
	//if (handle)
		//get_hge()->Resource_Free(handle);
	//handle = nullptr;
	handle.reset();
}

/////////////// RTexture //

void texture_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	texture_resource_t::ptr_t base;

	texture_resource_t::ptr_t rc( new texture_resource_t() );
	base = rm->find_texture(basename);
	if (base)
	{
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->mipmap = false;
	}
	rc->handle.reset();
	//rc->name = name;

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_FILENAME:
			sp->get_token();
			sp->get_token();
			rc->filename = sp->token_as_cstring();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		case TTPAR_MIPMAP:
			sp->get_token();
			sp->get_token();
			rc->mipmap = sp->token_as_bool();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	rm->m_textures[name] = rc;
}

HTEXTURE texture_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
		handle = hgeResourceManager::get_hge()->Texture_Load(filename.c_str(), 0, mipmap);
	return handle;
}

void texture_resource_t::Free()
{
	if (handle)
		hgeResourceManager::get_hge()->Texture_Free(handle);
	handle.reset();
}

/////////////// REffect //

void bass_effect_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	ScriptParseFileResource <bass_effect_resource_t> (rm, sp, name, rm->m_bass_effects, rm->find_bass_effect(basename) );
}

HEFFECT bass_effect_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
		handle = hgeResourceManager::get_hge()->Effect_Load(filename.c_str());
	return handle;
}

void bass_effect_resource_t::Free()
{
	if (handle)
		hgeResourceManager::get_hge()->Effect_Free(handle);
	handle.reset();
}

/////////////// RMusic //

void bass_music_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	//	ScriptParseFileResource(rm, sp, name, basename, new RMusic(), RES_MUSIC);
	bass_music_resource_t::ptr_t base;

	auto rc = bass_music_resource_t::ptr_t (new bass_music_resource_t());
	base = rm->find_bass_music(basename);

	if (base) {
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->amplify = 50;
	}

	rc->handle.reset();
	//strcpy(rc->name, name);

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_FILENAME:
			sp->get_token();
			sp->get_token();
			rc->filename = sp->token_as_cstring();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		case TTPAR_AMPLIFY:
			sp->get_token();
			sp->get_token();
			rc->amplify = sp->token_as_int();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	//AddRes(rm, RES_MUSIC, rc);
	rm->m_bass_musics[name] = rc;
}

HMUSIC bass_music_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
	{
		handle = hgeResourceManager::get_hge()->Music_Load(filename.c_str());
		hgeResourceManager::get_hge()->Music_SetAmplification(handle, amplify);
	}

	return handle;
}

void bass_music_resource_t::Free()
{
	if (handle)
		hgeResourceManager::get_hge()->Music_Free(handle);
	handle.reset();
}

/////////////// RStream //

void bass_stream_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	ScriptParseFileResource <bass_stream_resource_t> (rm, sp, name, rm->m_bass_streams, rm->find_bass_stream(basename));
}

HSTREAM bass_stream_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
		handle = hgeResourceManager::get_hge()->Stream_Load(filename.c_str());
	return handle;
}

void bass_stream_resource_t::Free()
{
	if (handle)
		hgeResourceManager::get_hge()->Stream_Free(handle);
	handle.reset();
}

/////////////// RTarget //

void target_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	target_resource_t::ptr_t base;

	auto rc = target_resource_t::ptr_t( new target_resource_t() );
	base = rm->find_target(basename);
	if (base) {
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->width = 256;
		rc->height = 256;
		rc->zbuffer = false;
	}
	rc->handle.reset();
	//strcpy(rc->name, name);

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_SIZE:
			sp->get_token();
			sp->get_token();
			rc->width = sp->token_as_int();
			sp->get_token();
			sp->get_token();
			rc->height = sp->token_as_int();
			break;

		case TTPAR_ZBUFFER:
			sp->get_token();
			sp->get_token();
			rc->zbuffer = sp->token_as_bool();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	//AddRes(rm, RES_TARGET, rc);
	rm->m_targets[name] = rc;
}

HTARGET target_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
		handle = hgeResourceManager::get_hge()->Target_Create(width, height, zbuffer);
	return handle;
}

void target_resource_t::Free()
{
	if (handle)
		hgeResourceManager::get_hge()->Target_Free(handle);
	handle.reset();
}

/////////////// RSprite //

void sprite_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	sprite_resource_t::ptr_t base;

	auto rc = sprite_resource_t::ptr_t(new sprite_resource_t());
	base = rm->find_sprite(basename);
	if (base) {
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->texname.clear();
		rc->tx = rc->ty = 0;
		rc->w = rc->h = 0;
		rc->hotx = rc->hoty = 0;
		rc->blend = BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
		rc->color = hge::COLOR_WHITE;
		rc->z = 0.5f;
		rc->bXFlip = false;
		rc->bYFlip = false;
		//		rc->x=rc->y=0;
		//		rc->scale=1.0f;
		//		rc->rotation=0.0f;
		//		rc->collision=HGECOL_RECT;
	}

	rc->handle = nullptr;
	//strcpy(rc->name, name);

	ScriptParseSpriteAnim(sp, rc.get(), false);
	//AddRes(rm, RES_SPRITE, rc);
	rm->m_sprites[name] = rc;
}

hgeSprite * sprite_resource_t::Get(hgeResourceManager *rm)
{
	hgeSprite *spr;
	if (!handle)
	{
		spr = new hgeSprite(rm->GetTexture(texname, resgroup), tx, ty, w, h);
		spr->SetColor(color);
		spr->SetZ(z);
		spr->SetBlendMode(blend);
		spr->SetHotSpot(hotx, hoty);
		spr->SetFlip(bXFlip, bYFlip);
		//		spr->MoveTo(x,y);
		//		spr->SetScale(scale);
		//		spr->SetRotation(rotation);
		//		spr->SetCollisionType(collision);

		handle = spr;
	}
	return handle;
}

void sprite_resource_t::Free()
{
	if (handle)
		delete handle;
	handle = nullptr;
}

/////////////// RAnimation //

void animation_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	animation_resource_t::ptr_t base;

	auto rc = animation_resource_t::ptr_t(new animation_resource_t());
	base = rm->find_animation(basename);
	if (base) {
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->texname.clear();
		rc->tx = rc->ty = 0;
		rc->w = rc->h = 0;
		rc->hotx = rc->hoty = 0;
		rc->blend = BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
		rc->color = hge::COLOR_WHITE;
		rc->z = 0.5f;
		rc->bXFlip = false;
		rc->bYFlip = false;
		//		rc->x=rc->y=0;
		//		rc->scale=1.0f;
		//		rc->rotation=0.0f;
		//		rc->collision=HGECOL_RECT;
		rc->frames = 1;
		rc->fps = 12.0f;
		rc->mode = (anim_mode_t) (HGEANIM_FWD | HGEANIM_LOOP);
	}

	rc->handle = nullptr;
	//strcpy(rc->name, name);

	ScriptParseSpriteAnim(sp, rc.get(), true);
	//AddRes(rm, RES_ANIMATION, rc);
	rm->m_animations[name] = rc;
}

hgeAnimation * animation_resource_t::GetAnimation(hgeResourceManager *rm)
{
	hgeAnimation *spr;
	if (!handle)
	{
		spr = new hgeAnimation(rm->GetTexture(texname, resgroup), frames, fps, tx, ty, w, h);
		spr->SetColor(color);
		spr->SetZ(z);
		spr->SetBlendMode(blend);
		spr->SetHotSpot(hotx, hoty);
		spr->SetFlip(bXFlip, bYFlip);
		//		spr->MoveTo(x,y);
		//		spr->SetScale(scale);
		//		spr->SetRotation(rotation);
		//		spr->SetCollisionType(collision);
		spr->SetMode(mode);

		handle = spr;
	}
	return (hgeAnimation *)handle;
}

void animation_resource_t::Free()
{
	if (handle)
		delete handle;
	handle = nullptr;
}

/////////////// RFont //

void font_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name, const std::string & basename)
{
	//RFont *rc, *base;

	auto rc = font_resource_t::ptr_t(new font_resource_t());
	auto base = rm->find_font(basename);
	if (base) {
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->mipmap = false;
		rc->filename.clear();
		rc->blend = BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
		rc->color = hge::COLOR_WHITE;
		rc->z = 0.5f;
		rc->scale = 1.0f;
		rc->proportion = 1.0f;
		rc->tracking = 0.0f;
		rc->spacing = 1.0f;
		rc->rotation = 0.0f;
	}
	rc->handle = nullptr;
	//strcpy(rc->name, name);

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_FILENAME:
			sp->get_token();
			sp->get_token();
			rc->filename = sp->token_as_cstring();
			break;

		case TTPAR_BLENDMODE:
			ScriptParseBlendMode(sp, &rc->blend);
			break;

		case TTPAR_COLOR:
			sp->get_token();
			sp->get_token();
			rc->color = sp->token_as_hex();
			break;

		case TTPAR_ZORDER:
			sp->get_token();
			sp->get_token();
			rc->z = sp->token_as_float();
			break;

		case TTPAR_SCALE:
			sp->get_token();
			sp->get_token();
			rc->scale = sp->token_as_float();
			break;

		case TTPAR_PROPORTION:
			sp->get_token();
			sp->get_token();
			rc->proportion = sp->token_as_float();
			break;

		case TTPAR_ROTATION:
			sp->get_token();
			sp->get_token();
			rc->rotation = sp->token_as_float();
			break;

		case TTPAR_TRACKING:
			sp->get_token();
			sp->get_token();
			rc->tracking = sp->token_as_float();
			break;

		case TTPAR_SPACING:
			sp->get_token();
			sp->get_token();
			rc->spacing = sp->token_as_float();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		case TTPAR_MIPMAP:
			sp->get_token();
			sp->get_token();
			rc->mipmap = sp->token_as_bool();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	//AddRes(rm, RES_FONT, rc);
	rm->m_fonts[name] = rc;
}

hgeFont * font_resource_t::Get(hgeResourceManager *rm)
{
	hgeFont *fnt;
	if (!handle)
	{
		fnt = new hgeFont(filename.c_str(), mipmap);
		fnt->SetColor(color);
		fnt->SetZ(z);
		fnt->SetBlendMode(blend);
		fnt->SetScale(scale);
		fnt->SetProportion(proportion);
		fnt->SetTracking(tracking);
		fnt->SetSpacing(spacing);
		fnt->SetRotation(rotation);

		handle = fnt;
	}
	return handle;
}

void font_resource_t::Free()
{
	if (handle)
		delete handle;
	handle = nullptr;
}

/////////////// RParticle //

void psystem_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	//RParticle *rc, *base;

	auto rc = psystem_resource_t::ptr_t(new psystem_resource_t());
	auto base = rm->find_particle_system(basename);
	if (base) {
		*rc = *base;
	}
	else
	{
		rc->resgroup = 0;
		rc->filename.clear();
		rc->spritename.clear();
	}
	rc->handle = nullptr;
	//strcpy(rc->name, name);

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_FILENAME:
			sp->get_token();
			sp->get_token();
			rc->filename = sp->token_as_cstring();
			break;

		case TTPAR_SPRITE:
			sp->get_token();
			sp->get_token();
			rc->spritename = sp->token_as_cstring();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	//AddRes(rm, RES_PARTICLE, rc);
	rm->m_psystems[name] = rc;
}

hgeParticleSystem * psystem_resource_t::Get(hgeResourceManager *rm)
{
	hgeParticleSystem *par;
	if (!handle)
	{
		par = new hgeParticleSystem(filename.c_str(), rm->GetSprite(spritename.c_str()));

		handle = par;
	}
	return handle;
}

void psystem_resource_t::Free()
{
	if (handle)
		delete handle;
	handle = nullptr;            
}

/////////////// RDistort //

void distort_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	//RDistort *rc, *base;

	auto rc = distort_resource_t::ptr_t(new distort_resource_t());
	auto base = rm->find_distortion_mesh(basename);
	if (base) {
		*rc = *base;
	} else
	{
		rc->resgroup = 0;
		rc->texname.clear();
		rc->tx = rc->ty = 0;
		rc->w = rc->h = 0;
		rc->cols = rc->rows = 2;
		rc->blend = BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
		rc->color = hge::COLOR_WHITE;
		rc->z = 0.5f;
	}
	rc->handle = nullptr;
	//strcpy(rc->name, name);

	while (ScriptSkipToNextParameter(sp, false))
	{
		switch (sp->tokentype)
		{
		case TTPAR_TEXTURE:
			sp->get_token();
			sp->get_token();
			rc->texname = sp->token_as_cstring();
			break;

		case TTPAR_RECT:
			sp->get_token();
			sp->get_token();
			rc->tx = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->ty = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->w = sp->token_as_float();
			sp->get_token();
			sp->get_token();
			rc->h = sp->token_as_float();
			break;

		case TTPAR_MESH:
			sp->get_token();
			sp->get_token();
			rc->cols = sp->token_as_int();
			sp->get_token();
			sp->get_token();
			rc->rows = sp->token_as_int();
			break;

		case TTPAR_BLENDMODE:
			ScriptParseBlendMode(sp, &rc->blend);
			break;

		case TTPAR_COLOR:
			sp->get_token();
			sp->get_token();
			rc->color = sp->token_as_hex();
			break;

		case TTPAR_ZORDER:
			sp->get_token();
			sp->get_token();
			rc->z = sp->token_as_float();
			break;

		case TTPAR_RESGROUP:
			sp->get_token();
			sp->get_token();
			rc->resgroup = sp->token_as_int();
			break;

		default:
			ScriptSkipToNextParameter(sp, true);
			break;
		}
	}

	//AddRes(rm, RES_DISTORT, rc);
	rm->m_distort_meshes[name] = rc;
}

hgeDistortionMesh * distort_resource_t::Get(hgeResourceManager *rm)
{
	hgeDistortionMesh * dis;
	if (!handle)
	{
		dis = new hgeDistortionMesh(cols, rows);
		dis->SetTexture(rm->GetTexture(texname, resgroup));
		dis->SetTextureRect(tx, ty, w, h);
		dis->SetBlendMode(blend);
		dis->Clear(color, z);

		handle = dis;
	}
	return handle;
}

void distort_resource_t::Free()
{
	if (handle)
		delete handle;
	handle = nullptr;
}

/////////////// RStringTable //

void string_table_resource_t::Parse(hgeResourceManager *rm, RScriptParser *sp, const std::string & name,
		const std::string & basename)
{
	ScriptParseFileResource <string_table_resource_t> (rm, sp, name, rm->m_string_tables, rm->find_string_table(basename));
}

hgeStringTable * string_table_resource_t::Get(hgeResourceManager *rm)
{
	if (!handle)
		handle = new hgeStringTable(filename);
	return handle;
}

void string_table_resource_t::Free()
{
	if (handle)
		delete handle;
	handle = nullptr;
}

} // namespace hge
