/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeFont helper class implementation
 */

#include <hgefont.h>
#include <stdlib.h>
#include <stdio.h>

namespace hge
{

const char FNTHEADERTAG[] = "[HGEFONT]";
const char FNTBITMAPTAG[] = "Bitmap";
const char FNTCHARTAG[] = "Char";

HGE * g_hgefont_hge = nullptr;
char g_hgefont_buffer[1024];

hgeFont::hgeFont(const char *szFont, bool bMipmap)
{
	void *data;
	uint32_t size;
	char *desc, *pdesc;
	char linebuf[256];
	char buf[MAX_PATH], *pbuf;
	char chr;
	int i, x, y, w, h, a, c;

	// Setup variables

	g_hgefont_hge = hgeCreate(HGE_VERSION);

	m_height = 0.0f;
	m_scale = 1.0f;
	m_proportion = 1.0f;
	m_rotation = 0.0f;
	m_tracking = 0.0f;
	m_spacing = 1.0f;
	m_texture = nullptr;

	m_depth = 0.5f;
	m_blending = BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
	m_color = 0xFFFFFFFF;

	ZeroMemory( &m_letters, sizeof(m_letters) );
	ZeroMemory( &m_pre_margin, sizeof(m_letters) );
	ZeroMemory( &m_post_margin, sizeof(m_letters) );

	// Load font description

	data = g_hgefont_hge->Resource_Load(szFont, &size);
	if (!data)
		return;

	desc = new char[size + 1];
	memcpy(desc, data, size);
	desc[size] = 0;
	g_hgefont_hge->Resource_Free(data);

	pdesc = _get_line(desc, linebuf);
	if (strcmp(linebuf, FNTHEADERTAG))
	{
		g_hgefont_hge->System_Log("Font %s has incorrect format.", szFont);
		delete[] desc;
		return;
	}

	// Parse font description

	while (pdesc = _get_line(pdesc, linebuf))
	{
		if (!strncmp(linebuf, FNTBITMAPTAG, sizeof(FNTBITMAPTAG) - 1))
		{
			strcpy(buf, szFont);
			pbuf = strrchr(buf, '\\');
			if (!pbuf)
				pbuf = strrchr(buf, '/');
			if (!pbuf)
				pbuf = buf;
			else
				pbuf++;
			if (!sscanf(linebuf, "Bitmap = %s", pbuf))
				continue;

			m_texture = g_hgefont_hge->Texture_Load(buf, 0, bMipmap);
			if (!m_texture)
			{
				delete[] desc;
				return;
			}
		}

		else if (!strncmp(linebuf, FNTCHARTAG, sizeof(FNTCHARTAG) - 1))
		{
			pbuf = strchr(linebuf, '=');
			if (!pbuf)
				continue;
			pbuf++;
			while (*pbuf == ' ')
				pbuf++;
			if (*pbuf == '\"')
			{
				pbuf++;
				i = (unsigned char) *pbuf++;
				pbuf++; // skip "
			}
			else
			{
				i = 0;
				while ((*pbuf >= '0' && *pbuf <= '9') || (*pbuf >= 'A' && *pbuf <= 'F') || (*pbuf
						>= 'a' && *pbuf <= 'f'))
				{
					chr = *pbuf;
					if (chr >= 'a')
						chr -= 'a' - ':';
					if (chr >= 'A')
						chr -= 'A' - ':';
					chr -= '0';
					if (chr > 0xF)
						chr = 0xF;
					i = (i << 4) | chr;
					pbuf++;
				}
				if (i < 0 || i > 255)
					continue;
			}
			sscanf(pbuf, " , %d , %d , %d , %d , %d , %d", &x, &y, &w, &h, &a, &c);

			m_letters[i] = new hgeSprite(m_texture, (float) x, (float) y, (float) w, (float) h);
			m_pre_margin[i] = (float) a;
			m_post_margin[i] = (float) c;
			if (h > m_height)
				m_height = (float) h;
		}
	}

	delete[] desc;
}

hgeFont::~hgeFont()
{
	for (int i = 0; i < 256; i++)
		if (m_letters[i])
			delete m_letters[i];
	if (m_texture)
		g_hgefont_hge->Texture_Free(m_texture);
	g_hgefont_hge->Release();
}

void hgeFont::Render(float x, float y, int align, const char *string)
{
	int i;
	float fx = x;

	align &= HGETEXT_HORZMASK;
	if (align == HGETEXT_RIGHT)
		fx -= GetStringWidth(string, false);
	if (align == HGETEXT_CENTER)
		fx -= int(GetStringWidth(string, false) / 2.0f);

	while (*string)
	{
		if (*string == '\n')
		{
			y += int(m_height * m_scale * m_spacing);
			fx = x;
			if (align == HGETEXT_RIGHT)
				fx -= GetStringWidth(string + 1, false);
			if (align == HGETEXT_CENTER)
				fx -= int(GetStringWidth(string + 1, false) / 2.0f);
		}
		else
		{
			i = (unsigned char) *string;
			if (!m_letters[i])
				i = '?';
			if (m_letters[i])
			{
				fx += m_pre_margin[i] * m_scale * m_proportion;
				m_letters[i]->RenderEx(fx, y, m_rotation, m_scale * m_proportion, m_scale);
				fx += (m_letters[i]->GetWidth() + m_post_margin[i] + m_tracking) * m_scale
						* m_proportion;
			}
		}
		string++;
	}
}

void hgeFont::printf(float x, float y, int align, const char *format, ...)
{
	char *pArg = (char *) &format + sizeof(format);

	_vsnprintf(g_hgefont_buffer, sizeof(g_hgefont_buffer) - 1, format, pArg);
	g_hgefont_buffer[sizeof(g_hgefont_buffer) - 1] = 0;
	//vsprintf(buffer, format, pArg);

	Render(x, y, align, g_hgefont_buffer);
}

void hgeFont::printfb(float x, float y, float w, float h, int align, const char *format, ...)
{
	char chr, *pbuf, *prevword, *linestart;
	int i, lines = 0;
	float tx, ty, hh, ww;
	char *pArg = (char *) &format + sizeof(format);

	_vsnprintf(g_hgefont_buffer, sizeof(g_hgefont_buffer) - 1, format, pArg);
	g_hgefont_buffer[sizeof(g_hgefont_buffer) - 1] = 0;
	//vsprintf(buffer, format, pArg);

	linestart = g_hgefont_buffer;
	pbuf = g_hgefont_buffer;
	prevword = 0;

	for (;;)
	{
		i = 0;
		while (pbuf[i] && pbuf[i] != ' ' && pbuf[i] != '\n')
			i++;

		chr = pbuf[i];
		pbuf[i] = 0;
		ww = GetStringWidth(linestart);
		pbuf[i] = chr;

		if (ww > w)
		{
			if (pbuf == linestart)
			{
				pbuf[i] = '\n';
				linestart = &pbuf[i + 1];
			}
			else
			{
				*prevword = '\n';
				linestart = prevword + 1;
			}

			lines++;
		}

		if (pbuf[i] == '\n')
		{
			prevword = &pbuf[i];
			linestart = &pbuf[i + 1];
			pbuf = &pbuf[i + 1];
			lines++;
			continue;
		}

		if (!pbuf[i])
		{
			lines++;
			break;
		}

		prevword = &pbuf[i];
		pbuf = &pbuf[i + 1];
	}

	tx = x;
	ty = y;
	hh = m_height * m_spacing * m_scale * lines;

	switch (align & HGETEXT_HORZMASK)
	{
	case HGETEXT_LEFT:
		break;
	case HGETEXT_RIGHT:
		tx += w;
		break;
	case HGETEXT_CENTER:
		tx += int(w / 2);
		break;
	}

	switch (align & HGETEXT_VERTMASK)
	{
	case HGETEXT_TOP:
		break;
	case HGETEXT_BOTTOM:
		ty += h - hh;
		break;
	case HGETEXT_MIDDLE:
		ty += int((h - hh) / 2);
		break;
	}

	Render(tx, ty, align, g_hgefont_buffer);
}

float hgeFont::GetStringWidth(const char *string, bool bMultiline) const
{
	int i;
	float linew, w = 0;

	while (*string)
	{
		linew = 0;

		while (*string && *string != '\n')
		{
			i = (unsigned char) *string;
			if (!m_letters[i])
				i = '?';
			if (m_letters[i])
				linew += m_letters[i]->GetWidth() + m_pre_margin[i] + m_post_margin[i] + m_tracking;

			string++;
		}

		if (!bMultiline)
			return linew * m_scale * m_proportion;

		if (linew > w)
			w = linew;

		while (*string == '\n' || *string == '\r')
			string++;
	}

	return w * m_scale * m_proportion;
}

void hgeFont::SetColor(uint32_t col)
{
	m_color = col;

	for (int i = 0; i < 256; i++)
		if (m_letters[i])
			m_letters[i]->SetColor(col);
}

void hgeFont::SetZ(float z)
{
	m_depth = z;

	for (int i = 0; i < 256; i++)
		if (m_letters[i])
			m_letters[i]->SetZ(z);
}

void hgeFont::SetBlendMode(uint32_t blend)
{
	m_blending = blend;

	for (int i = 0; i < 256; i++)
		if (m_letters[i])
			m_letters[i]->SetBlendMode(blend);
}

char *hgeFont::_get_line(char *file, char *line)
{
	int i = 0;

	if (!file[i])
		return 0;

	while (file[i] && file[i] != '\n' && file[i] != '\r')
	{
		line[i] = file[i];
		i++;
	}
	line[i] = 0;

	while (file[i] && (file[i] == '\n' || file[i] == '\r'))
		i++;

	return file + i;
}

HGE * hgeFont::get_hge()
{
	return g_hgefont_hge;
}

void hgeFont::SetScale(float scale)
{
	m_scale = scale;
}

void hgeFont::SetProportion(float prop)
{
	m_proportion = prop;
}

void hgeFont::SetRotation(float rot)
{
	m_rotation = rot;
}

void hgeFont::SetTracking(float tracking)
{
	m_tracking = tracking;
}

void hgeFont::SetSpacing(float spacing)
{
	m_spacing = spacing;
}

uint32_t hgeFont::GetColor() const
{
	return m_color;
}

float hgeFont::GetZ() const
{
	return m_depth;
}

uint32_t hgeFont::GetBlendMode() const
{
	return m_blending;
}

float hgeFont::GetScale() const
{
	return m_scale;
}

float hgeFont::GetProportion() const
{
	return m_proportion;
}

float hgeFont::GetRotation() const
{
	return m_rotation;
}

float hgeFont::GetTracking() const
{
	return m_tracking;
}

float hgeFont::GetSpacing() const
{
	return m_spacing;
}

hgeSprite* hgeFont::GetSprite(char chr) const
{
	return m_letters[(unsigned char) chr];
}

float hgeFont::GetPreWidth(char chr) const
{
	return m_pre_margin[(unsigned char) chr];
}

float hgeFont::GetPostWidth(char chr) const
{
	return m_post_margin[(unsigned char) chr];
}

float hgeFont::GetHeight() const
{
	return m_height;
}

} // namespace hge
