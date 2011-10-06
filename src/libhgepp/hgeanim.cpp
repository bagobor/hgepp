/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeAnimation helper class implementation
 */

#include <hgeanim.h>

namespace hge
{

hgeAnimation::hgeAnimation(HTEXTURE tex, int nframes, float FPS, float x, float y, float w, float h) :
	hgeSprite(tex, x, y, w, h)
{
	m_orig_width = get_hge()->Texture_GetWidth(tex, true);

	m_since_last_frame = -1.0f;
	m_speed = 1.0f / FPS;
	m_playing_flag = false;
	m_frame_count = nframes;

	m_play_mode = (anim_mode_t) (HGEANIM_FWD | HGEANIM_LOOP);
	m_delta = 1;
	SetFrame(0);
}

hgeAnimation::hgeAnimation(const hgeAnimation & anim) :
	hgeSprite(anim)
{
	// Copy hgeAnimation parameters: 
	this->m_orig_width = anim.m_orig_width;
	this->m_playing_flag = anim.m_playing_flag;
	this->m_speed = anim.m_speed;
	this->m_since_last_frame = anim.m_since_last_frame;
	this->m_play_mode = anim.m_play_mode;
	this->m_delta = anim.m_delta;
	this->m_frame_count = anim.m_frame_count;
	this->m_cur_frame = anim.m_cur_frame;
}

void hgeAnimation::SetMode(anim_mode_t mode)
{
	m_play_mode = mode;

	if (mode & HGEANIM_REV)
	{
		m_delta = -1;
		SetFrame(m_frame_count - 1);
	}
	else
	{
		m_delta = 1;
		SetFrame(0);
	}
}

void hgeAnimation::Play()
{
	m_playing_flag = true;
	m_since_last_frame = -1.0f;
	if (m_play_mode & HGEANIM_REV)
	{
		m_delta = -1;
		SetFrame(m_frame_count - 1);
	}
	else
	{
		m_delta = 1;
		SetFrame(0);
	}
}

void hgeAnimation::Update(float fDeltaTime)
{
	if (!m_playing_flag)
		return;

	if (m_since_last_frame == -1.0f)
		m_since_last_frame = 0.0f;
	else
		m_since_last_frame += fDeltaTime;

	while (m_since_last_frame >= m_speed)
	{
		m_since_last_frame -= m_speed;

		if (m_cur_frame + m_delta == m_frame_count)
		{
			switch (m_play_mode)
			{
			case HGEANIM_FWD:
			case HGEANIM_REV | HGEANIM_PINGPONG:
				m_playing_flag = false;
				break;

			case HGEANIM_FWD | HGEANIM_PINGPONG:
			case HGEANIM_FWD | HGEANIM_PINGPONG | HGEANIM_LOOP:
			case HGEANIM_REV | HGEANIM_PINGPONG | HGEANIM_LOOP:
				m_delta = -m_delta;
				break;
			}
		}
		else if (m_cur_frame + m_delta < 0)
		{
			switch (m_play_mode)
			{
			case HGEANIM_REV:
			case HGEANIM_FWD | HGEANIM_PINGPONG:
				m_playing_flag = false;
				break;

			case HGEANIM_REV | HGEANIM_PINGPONG:
			case HGEANIM_REV | HGEANIM_PINGPONG | HGEANIM_LOOP:
			case HGEANIM_FWD | HGEANIM_PINGPONG | HGEANIM_LOOP:
				m_delta = -m_delta;
				break;
			}
		}

		if (m_playing_flag)
			SetFrame(m_cur_frame + m_delta);
	}
}

void hgeAnimation::SetFrame(int n)
{
	float tx1, ty1, tx2, ty2;
	bool bX, bY, bHS;
	int ncols = int(m_orig_width) / int(m_width);

	n = n % m_frame_count;
	if (n < 0)
		n = m_frame_count + n;
	m_cur_frame = n;

	// calculate texture coords for frame n
	ty1 = m_tex_y;
	tx1 = m_tex_x + n * m_width;

	if (tx1 > m_orig_width - m_width)
	{
		n -= int(m_orig_width - m_tex_x) / int(m_width);
		tx1 = m_width * (n % ncols);
		ty1 += m_height * (1 + n / ncols);
	}

	tx2 = tx1 + m_width;
	ty2 = ty1 + m_height;

	tx1 /= m_tex_width;
	ty1 /= m_tex_height;
	tx2 /= m_tex_width;
	ty2 /= m_tex_height;

	m_quad.v[0].tx = tx1;
	m_quad.v[0].ty = ty1;
	m_quad.v[1].tx = tx2;
	m_quad.v[1].ty = ty1;
	m_quad.v[2].tx = tx2;
	m_quad.v[2].ty = ty2;
	m_quad.v[3].tx = tx1;
	m_quad.v[3].ty = ty2;

	bX = m_xflip;
	bY = m_yflip;
	bHS = m_hsflip;
	m_xflip = false;
	m_yflip = false;
	SetFlip(bX, bY, bHS);
}

void hgeAnimation::SetTexture(HTEXTURE tex)
{
	hgeSprite::SetTexture(tex);
	m_orig_width = get_hge()->Texture_GetWidth(tex, true);
}

void hgeAnimation::SetTextureRect(float x1, float y1, float x2, float y2)
{
	hgeSprite::SetTextureRect(x1, y1, x2, y2);
	SetFrame(m_cur_frame);
}

void hgeAnimation::SetFrames(int n)
{
	m_frame_count = n;
}

void hgeAnimation::SetSpeed(float FPS)
{
	m_speed = 1.0f / FPS;
}

anim_mode_t hgeAnimation::GetMode() const
{
	return m_play_mode;
}

float hgeAnimation::GetSpeed() const
{
	return 1.0f / m_speed;
}

int hgeAnimation::GetFrame() const
{
	return m_cur_frame;
}

int hgeAnimation::GetFrames() const
{
	return m_frame_count;
}

bool hgeAnimation::IsPlaying() const
{
	return m_playing_flag;
}

void hgeAnimation::Resume()
{
	m_playing_flag = true;
}

void hgeAnimation::Stop()
{
	m_playing_flag = false;
}

} // namespace hge
