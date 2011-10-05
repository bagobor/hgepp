/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeAnimation helper class header
 */
#pragma once

#include <hge.h>
#include <hgesprite.h>

namespace hge {

#define HGEANIM_FWD			0
#define HGEANIM_REV			1
#define HGEANIM_PINGPONG	2
#define HGEANIM_NOPINGPONG	0
#define HGEANIM_LOOP		4
#define HGEANIM_NOLOOP		0


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
	void		SetMode(int mode);
	void		SetSpeed(float FPS);
	void		SetFrame(int n);
	void		SetFrames(int n);

	int			GetMode() const;
	float		GetSpeed() const;
	int			GetFrame() const;
	int			GetFrames() const;

private:
	hgeAnimation();

	int			orig_width;

	bool		bPlaying;

	float		fSpeed;
	float		fSinceLastFrame;

	int			Mode;
	int			nDelta;
	int			nFrames;
	int			nCurFrame;
};

} // namespace hge
