/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeAnimation helper class header
 */
#pragma once

#include <hge.h>
#include <hgesprite.h>

namespace hge {

// TODO: replace this with something more appropriate than macros or an enum. Struct with bit fields?
typedef enum {
	HGEANIM_FWD			= 0,
	HGEANIM_REV			= 1,
	HGEANIM_PINGPONG	= 2,
	HGEANIM_NOPINGPONG	= 0,
	HGEANIM_LOOP		= 4,
	HGEANIM_NOLOOP		= 0
} anim_mode_t;

// typedef struct {
// 	bool reverse:1;
// 	bool pingpong:1;
// 	bool loop:1;
// } anim_mode_t;

/*
** HGE Animation class
*/
class HGE_EXPORT hgeAnimation : public hgeSprite
{
public:
	hgeAnimation(HTEXTURE tex, int nframes, float FPS, float x, float y, float w, float h);
	hgeAnimation(const hgeAnimation &anim);
	
	void		Play();
	void		Stop();
	void		Resume();
	void		Update(float fDeltaTime);
	bool		IsPlaying() const;

	void		SetTexture(HTEXTURE tex);
	void		SetTextureRect(float x1, float y1, float x2, float y2);
	void		SetMode(anim_mode_t mode);
	void		SetSpeed(float FPS);
	void		SetFrame(int n);
	void		SetFrames(int n);

	anim_mode_t GetMode() const;
	float		GetSpeed() const;
	int			GetFrame() const;
	int			GetFrames() const;

private:
	hgeAnimation();

	int			m_orig_width;

	bool		m_playing_flag;

	float		m_speed;
	float		m_since_last_frame;

	anim_mode_t	m_play_mode;
	int			m_delta;
	int			m_frame_count;
	int			m_cur_frame;
};

} // namespace hge
