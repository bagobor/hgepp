/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeGUI default controls implementation
 */

#include <hgeguictrls.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace hge
{

/*
 ** hgeGUIText
 */

hgeGUIText::hgeGUIText(int _id, float x, float y, float w, float h, hgeFont *fnt)
{
	m_object_id = _id;
	m_is_static = true;
	m_is_visible = true;
	m_is_enabled = true;
	m_rect.Set(x, y, x + w, y + h);

	m_font = fnt;
	m_draw_x = x;
	m_draw_y = y + (h - fnt->GetHeight()) / 2.0f;

	//m_text.clear();
}

void hgeGUIText::SetMode(int _align)
{
	m_align = _align;
	if (m_align == HGETEXT_RIGHT)
		m_draw_x = m_rect.x2;
	else if (m_align == HGETEXT_CENTER)
		m_draw_x = (m_rect.x1 + m_rect.x2) / 2.0f;
	else
		m_draw_x = m_rect.x1;
}

void hgeGUIText::SetText(const char *_text)
{
	m_text = _text;
}

void hgeGUIText::printf(const char *format, ...)
{
	char tmp[1024];

	auto r = _snprintf( tmp, sizeof tmp, format, (char *)&format+sizeof(format) );
	if (r == -1)
	{
		m_text = std::string(tmp, sizeof tmp);
	}
	else
	{
		m_text = std::string(tmp, r);
	}
}

void hgeGUIText::Render()
{
	m_font->SetColor(m_color);
	m_font->Render(m_draw_x, m_draw_y, m_align, m_text.c_str());
}

/*
 ** hgeGUIButton
 */

hgeGUIButton::hgeGUIButton(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx,
		float ty)
{
	m_object_id = _id;
	m_is_static = false;
	m_is_visible = true;
	m_is_enabled = true;
	m_rect.Set(x, y, x + w, y + h);

	m_pressed = false;
	m_trigger = false;

	m_sprite_up = new hgeSprite(tex, tx, ty, w, h);
	m_sprite_down = new hgeSprite(tex, tx + w, ty, w, h);
}

hgeGUIButton::~hgeGUIButton()
{
	if (m_sprite_up)
		delete m_sprite_up;
	if (m_sprite_down)
		delete m_sprite_down;
}

void hgeGUIButton::Render()
{
	if (m_pressed)
		m_sprite_down->Render(m_rect.x1, m_rect.y1);
	else
		m_sprite_up->Render(m_rect.x1, m_rect.y1);
}

bool hgeGUIButton::MouseLButton(bool bDown)
{
	if (bDown)
	{
		m_old_state = m_pressed;
		m_pressed = true;
		return false;
	}
	else
	{
		if (m_trigger)
			m_pressed = !m_old_state;
		else
			m_pressed = false;
		return true;
	}
}

void hgeGUIButton::SetMode(bool _bTrigger)
{
	m_trigger = _bTrigger;
}

void hgeGUIButton::SetState(bool _bPressed)
{
	m_pressed = _bPressed;
}

bool hgeGUIButton::GetState() const
{
	return m_pressed;
}

/*
 ** hgeGUISlider
 */

hgeGUISlider::hgeGUISlider(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx,
		float ty, float sw, float sh, bool vertical)
{
	m_object_id = _id;
	m_is_static = false;
	m_is_visible = true;
	m_is_enabled = true;
	m_pressed = false;
	m_vertical = vertical;
	m_rect.Set(x, y, x + w, y + h);

	m_slider_mode = HGESLIDER_BAR;
	m_min = 0;
	m_max = 100;
	m_value = 50;
	m_slider_width = sw;
	m_slider_height = sh;

	m_slider_sprite = new hgeSprite(tex, tx, ty, sw, sh);
}

hgeGUISlider::~hgeGUISlider()
{
	if (m_slider_sprite)
		delete m_slider_sprite;
}

void hgeGUISlider::SetValue(float value)
{
	if (value < m_min)
		m_value = m_min;
	else if (value > m_max)
		m_value = m_max;
	else
		m_value = value;
}

void hgeGUISlider::Render()
{
	float xx, yy;
	float x1, y1, x2, y2;

	xx = m_rect.x1 + (m_rect.x2 - m_rect.x1) * (m_value - m_min) / (m_max - m_min);
	yy = m_rect.y1 + (m_rect.y2 - m_rect.y1) * (m_value - m_min) / (m_max - m_min);

	if (m_vertical)
		switch (m_slider_mode)
		{
		case HGESLIDER_BAR:
			x1 = m_rect.x1;
			y1 = m_rect.y1;
			x2 = m_rect.x2;
			y2 = yy;
			break;
		case HGESLIDER_BARRELATIVE:
			x1 = m_rect.x1;
			y1 = (m_rect.y1 + m_rect.y2) / 2;
			x2 = m_rect.x2;
			y2 = yy;
			break;
		case HGESLIDER_SLIDER:
			x1 = (m_rect.x1 + m_rect.x2 - m_slider_width) / 2;
			y1 = yy - m_slider_height / 2;
			x2 = (m_rect.x1 + m_rect.x2 + m_slider_width) / 2;
			y2 = yy + m_slider_height / 2;
			break;
		}
	else
		switch (m_slider_mode)
		{
		case HGESLIDER_BAR:
			x1 = m_rect.x1;
			y1 = m_rect.y1;
			x2 = xx;
			y2 = m_rect.y2;
			break;
		case HGESLIDER_BARRELATIVE:
			x1 = (m_rect.x1 + m_rect.x2) / 2;
			y1 = m_rect.y1;
			x2 = xx;
			y2 = m_rect.y2;
			break;
		case HGESLIDER_SLIDER:
			x1 = xx - m_slider_width / 2;
			y1 = (m_rect.y1 + m_rect.y2 - m_slider_height) / 2;
			x2 = xx + m_slider_width / 2;
			y2 = (m_rect.y1 + m_rect.y2 + m_slider_height) / 2;
			break;
		}

	m_slider_sprite->RenderStretch(x1, y1, x2, y2);
}

bool hgeGUISlider::MouseLButton(bool bDown)
{
	m_pressed = bDown;
	return false;
}

bool hgeGUISlider::MouseMove(float x, float y)
{
	if (m_pressed)
	{
		if (m_vertical)
		{
			if (y > m_rect.y2 - m_rect.y1)
				y = m_rect.y2 - m_rect.y1;
			if (y < 0)
				y = 0;
			m_value = m_min + (m_max - m_min) * y / (m_rect.y2 - m_rect.y1);
		}
		else
		{
			if (x > m_rect.x2 - m_rect.x1)
				x = m_rect.x2 - m_rect.x1;
			if (x < 0)
				x = 0;
			m_value = m_min + (m_max - m_min) * x / (m_rect.x2 - m_rect.x1);
		}
		return true;
	}

	return false;
}

void hgeGUISlider::SetMode(float minv, float maxv, int _mode)
{
	m_min = minv;
	m_max = maxv;
	m_slider_mode = _mode;
}

float hgeGUISlider::GetValue() const
{
	return m_value;
}

/*
 ** hgeGUIListbox
 */

hgeGUIListbox::hgeGUIListbox(int _id, float x, float y, float w, float h, hgeFont *fnt,
		uint32_t tColor, uint32_t thColor, uint32_t hColor)
{
	m_object_id = _id;
	m_is_static = false;
	m_is_visible = true;
	m_is_enabled = true;
	m_rect.Set(x, y, x + w, y + h);
	m_font = fnt;
	m_hilite_sprite = new hgeSprite(HTEXTURE(), 0, 0, w, fnt->GetHeight());
	m_hilite_sprite->SetColor(hColor);
	m_text_color = tColor;
	m_hi_text_color = thColor;
	m_items = 0;
	m_item_count = 0;

	m_selected_item = 0;
	m_top_item = 0;
	m_mouse_x = 0;
	m_mouse_y = 0;
}

hgeGUIListbox::~hgeGUIListbox()
{
	Clear();
	if (m_hilite_sprite)
		delete m_hilite_sprite;
}

int hgeGUIListbox::AddItem(char *item)
{
	hgeGUIListboxItem *pItem = m_items, *pPrev = 0, *pNew;

	pNew = new hgeGUIListboxItem;
	//memcpy(pNew->text, item, min(sizeof(pNew->text), strlen(item)+1));
	//pNew->text[sizeof(pNew->text)-1]='\0';
	pNew->text = item;
	pNew->next = 0;

	while (pItem)
	{
		pPrev = pItem;
		pItem = pItem->next;
	}

	if (pPrev)
		pPrev->next = pNew;
	else
		m_items = pNew;
	m_item_count++;

	return m_item_count - 1;
}

void hgeGUIListbox::DeleteItem(int n)
{
	int i;
	hgeGUIListboxItem *pItem = m_items, *pPrev = 0;

	if (n < 0 || n >= GetNumItems())
		return;

	for (i = 0; i < n; i++)
	{
		pPrev = pItem;
		pItem = pItem->next;
	}

	if (pPrev)
		pPrev->next = pItem->next;
	else
		m_items = pItem->next;

	delete pItem;
	m_item_count--;
}

const char *hgeGUIListbox::GetItemText(int n) const
{
	int i;
	hgeGUIListboxItem *pItem = m_items;

	if (n < 0 || n >= GetNumItems())
		return 0;

	for (i = 0; i < n; i++)
		pItem = pItem->next;

	return pItem->text.c_str();
}

void hgeGUIListbox::Clear()
{
	hgeGUIListboxItem *pItem = m_items, *pNext;

	while (pItem)
	{
		pNext = pItem->next;
		delete pItem;
		pItem = pNext;
	}

	m_items = 0;
	m_item_count = 0;
}

void hgeGUIListbox::Render()
{
	int i;
	hgeGUIListboxItem *pItem = m_items;

	for (i = 0; i < m_top_item; i++)
		pItem = pItem->next;
	for (i = 0; i < GetNumRows(); i++)
	{
		if (i >= m_item_count)
			return;

		if (m_top_item + i == m_selected_item)
		{
			m_hilite_sprite->Render(m_rect.x1, m_rect.y1 + i * m_font->GetHeight());
			m_font->SetColor(m_hi_text_color);
		}
		else
			m_font->SetColor(m_text_color);

		m_font->Render(m_rect.x1 + 3, m_rect.y1 + i * m_font->GetHeight(), HGETEXT_LEFT,
				pItem->text.c_str());
		pItem = pItem->next;
	}
}

bool hgeGUIListbox::MouseLButton(bool bDown)
{
	int nItem;

	if (bDown)
	{
		nItem = m_top_item + int(m_mouse_y) / int(m_font->GetHeight());
		if (nItem < m_item_count)
		{
			m_selected_item = nItem;
			return true;
		}
	}
	return false;
}

bool hgeGUIListbox::MouseWheel(int nNotches)
{
	m_top_item -= nNotches;
	if (m_top_item < 0)
		m_top_item = 0;
	if (m_top_item > GetNumItems() - GetNumRows())
		m_top_item = GetNumItems() - GetNumRows();

	return true;
}

bool hgeGUIListbox::KeyClick(int key, int chr)
{
	switch (key)
	{
	case HGEK_DOWN:
		if (m_selected_item < m_item_count - 1)
		{
			m_selected_item++;
			if (m_selected_item > m_top_item + GetNumRows() - 1)
				m_top_item = m_selected_item - GetNumRows() + 1;
			return true;
		}
		break;

	case HGEK_UP:
		if (m_selected_item > 0)
		{
			m_selected_item--;
			if (m_selected_item < m_top_item)
				m_top_item = m_selected_item;
			return true;
		}
		break;
	}
	return false;
}

int hgeGUIListbox::GetSelectedItem() const
{
	return m_selected_item;
}

void hgeGUIListbox::SetSelectedItem(int n)
{
	if (n >= 0 && n < GetNumItems())
		m_selected_item = n;
}

int hgeGUIListbox::GetTopItem() const
{
	return m_top_item;
}

void hgeGUIListbox::SetTopItem(int n)
{
	if (n >= 0 && n <= GetNumItems() - GetNumRows())
		m_top_item = n;
}

int hgeGUIListbox::GetNumItems() const
{
	return m_item_count;
}

int hgeGUIListbox::GetNumRows() const
{
	return int((m_rect.y2 - m_rect.y1) / m_font->GetHeight());
}

bool hgeGUIListbox::MouseMove(float x, float y)
{
	m_mouse_x = x;
	m_mouse_y = y;
	return false;
}

} // namespace hge
