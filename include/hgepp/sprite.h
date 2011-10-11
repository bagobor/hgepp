/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeSprite helper class header
 */
#pragma once

#include <hgepp.h>
#include <hgepp/rect.h>

namespace hge {

/*
** HGE Sprite class
*/
class HGE_EXPORT hgeSprite
{
public:
	hgeSprite(HTEXTURE tex, float x, float y, float w, float h);
	hgeSprite(const hgeSprite &spr);
	~hgeSprite();

	void Render(float x, float y);
	void RenderEx(float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f);
	void RenderStretch(float x1, float y1, float x2, float y2);
	void Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

	void SetTexture(HTEXTURE tex);
	void SetTextureRect(float x, float y, float w, float h, bool adjSize = true);
	void SetColor(uint32_t col, int i = -1);
	void SetZ(float z, int i = -1);
	void SetBlendMode(uint32_t blend);
	void SetHotSpot(float x, float y);
	void SetFlip(bool bX, bool bY, bool bHotSpot = false);

	HTEXTURE GetTexture() const;
	void GetTextureRect(float *x, float *y, float *w, float *h) const;
	uint32_t GetColor(int i = 0) const;
	float GetZ(int i = 0) const;
	uint32_t GetBlendMode() const;
	void GetHotSpot(float *x, float *y) const;
	void GetFlip(bool *bX, bool *bY) const;

	float GetWidth() const;
	float GetHeight() const;
	hgeRect* GetBoundingBox(float x, float y, hgeRect *rect) const;
	hgeRect* GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale,
			hgeRect *rect) const;

protected:
	hgeSprite();
	static HGE * get_hge();

	hgeQuad m_quad;
	float m_tex_x;
	float m_tex_y;
	float m_width;
	float m_height;
	float m_tex_width;
	float m_tex_height;
	float m_hotx;
	float m_hoty;
	bool m_xflip;
	bool m_yflip;
	bool m_hsflip;
};


/*
** HGE Animation class
*/
class HGE_EXPORT hgeAnimation : public hgeSprite
{
public:
	hgeAnimation(HTEXTURE tex, int nframes, float FPS, float x, float y, float w, float h);
	hgeAnimation(const hgeAnimation &anim);
	
	void Play();
	void Stop();
	void Resume();
	void Update(float fDeltaTime);
	bool IsPlaying() const;

	void SetTexture(HTEXTURE tex);
	void SetTextureRect(float x1, float y1, float x2, float y2);
	void SetMode(anim_mode_t mode);
	void SetSpeed(float FPS);
	void SetFrame(int n);
	void SetFrames(int n);

	anim_mode_t GetMode() const;
	float GetSpeed() const;
	int GetFrame() const;
	int GetFrames() const;

private:
	hgeAnimation();

	int m_orig_width;

	bool m_playing_flag;

	float m_speed;
	float m_since_last_frame;

	anim_mode_t m_play_mode;
	int m_delta;
	int m_frame_count;
	int m_cur_frame;
};

} // namespace hge
