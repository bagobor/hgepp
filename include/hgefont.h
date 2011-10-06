/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeFont helper class header
 */
#pragma once

#include <hge.h>
#include <hgesprite.h>

namespace hge {

#define HGETEXT_LEFT		0
#define HGETEXT_RIGHT		1
#define HGETEXT_CENTER		2
#define HGETEXT_HORZMASK	0x03

#define HGETEXT_TOP			0
#define HGETEXT_BOTTOM		4
#define HGETEXT_MIDDLE		8
#define HGETEXT_VERTMASK	0x0C

/*
** HGE Font class
*/
class HGE_EXPORT hgeFont
{
public:
	hgeFont(const char *filename, bool bMipmap=false);
	~hgeFont();

	void Render(float x, float y, int align, const char *string);
	void printf(float x, float y, int align, const char *format, ...);
	void printfb(float x, float y, float w, float h, int align, const char *format, ...);

	void SetColor(uint32_t col);
	void SetZ(float z);
	void SetBlendMode(uint32_t blend);
	void SetScale(float scale);
	void SetProportion(float prop);
	void SetRotation(float rot);
	void SetTracking(float tracking);
	void SetSpacing(float spacing);

	uint32_t GetColor() const;
	float GetZ() const;
	uint32_t GetBlendMode() const;
	float GetScale() const;
	float GetProportion() const;
	float GetRotation() const;
	float GetTracking() const;
	float GetSpacing() const;

	hgeSprite* GetSprite(char chr) const;
	float GetPreWidth(char chr) const;
	float GetPostWidth(char chr) const;
	float GetHeight() const;
	float GetStringWidth(const char *string, bool bMultiline = true) const;

private:
	hgeFont();
	hgeFont(const hgeFont &fnt);
	hgeFont& operator=(const hgeFont &fnt);

	char* _get_line(char *file, char *line);

	//static HGE	*g_hgefont_hge;
	static HGE * get_hge();

	//static char	g_hgefont_buffer[1024];

	HTEXTURE m_texture;
	hgeSprite* m_letters[256];
	float m_pre_margin[256];
	float m_post_margin[256];
	float m_height;
	float m_scale;
	float m_proportion;
	float m_rotation;
	float m_tracking;
	float m_spacing;

	uint32_t m_color;
	float m_depth;
	uint32_t m_blending;
};

} // namespace hge
