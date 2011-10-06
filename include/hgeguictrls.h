/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeGUI default controls header
 */

#pragma once

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgerect.h>
#include <hgegui.h>

#include <string>
// we use STL but we avoid exporting it, so get rid of warning
#pragma warning(disable:4251)

namespace hge {


/*
** hgeGUIText
*/
class HGE_EXPORT hgeGUIText : public hgeGUIObject
{
public:
	hgeGUIText(int id, float x, float y, float w, float h, hgeFont *fnt);

	void			SetMode(int _align);
	void			SetText(const char *_text);
	void			printf(const char *format, ...);

	virtual void	Render();

private:
	hgeFont *		m_font;
	float			m_draw_x;
	float			m_draw_y;
	int				m_align;
	//char			text[256];
	std::string		m_text;
};


/*
** hgeGUIButton
*/
class HGE_EXPORT hgeGUIButton : public hgeGUIObject
{
public:
	hgeGUIButton(int id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty);
	virtual			~hgeGUIButton();

	void			SetMode(bool _bTrigger);
	void			SetState(bool _bPressed);
	bool			GetState() const;

	virtual void	Render();
	virtual bool	MouseLButton(bool bDown);

private:
	bool			m_trigger;
	bool			m_pressed;
	bool			m_old_state;
	hgeSprite		* m_sprite_up;
	hgeSprite		* m_sprite_down;
};


/*
** hgeGUISlider
*/
enum {
	HGESLIDER_BAR			= 0,
	HGESLIDER_BARRELATIVE	= 1,
	HGESLIDER_SLIDER		= 2
};

class HGE_EXPORT hgeGUISlider : public hgeGUIObject
{
public:
	hgeGUISlider(int id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty, float sw, float sh, bool vertical=false);
	virtual			~hgeGUISlider();

	void			SetMode(float minv, float maxv, int mode);
	void			SetValue(float value);
	float			GetValue() const;

	virtual void	Render();
	virtual bool	MouseMove(float x, float y);
	virtual bool	MouseLButton(bool bDown);

private:
	bool			m_pressed;
	bool			m_vertical;
	int				m_slider_mode;
	float			m_min;
	float			m_max;
	float			m_value;
	float			m_slider_width;
	float			m_slider_height;
	hgeSprite		* m_slider_sprite;
};


/*
** hgeGUIListbox
*/
struct hgeGUIListboxItem
{
	//char				text[64];
	std::string			text;
	hgeGUIListboxItem	* next;
};

class HGE_EXPORT hgeGUIListbox : public hgeGUIObject
{
public:
	hgeGUIListbox(int id, float x, float y, float w, float h, hgeFont *fnt, uint32_t tColor, uint32_t thColor, uint32_t hColor);
	virtual			~hgeGUIListbox();

	int				AddItem(char *item);
	void			DeleteItem(int n);
	int				GetSelectedItem() const;
	void			SetSelectedItem(int n);
	int				GetTopItem() const;
	void			SetTopItem(int n);

	const char		* GetItemText(int n) const;
	int				GetNumItems() const;
	int				GetNumRows() const;
	void			Clear();

	virtual void	Render();
	virtual bool	MouseMove(float x, float y);
	virtual bool	MouseLButton(bool bDown);
	virtual bool	MouseWheel(int nNotches);
	virtual bool	KeyClick(int key, int chr);

private:
	hgeSprite		* m_hilite_sprite;
	hgeFont			* m_font;
	uint32_t		m_text_color;
	uint32_t		m_hi_text_color;

	int					m_item_count;
	int					m_selected_item;
	int					m_top_item;
	float				m_mouse_x;
	float				m_mouse_y;
	hgeGUIListboxItem	* m_items;
};


inline bool hgeButtonGetState( hgeGUI * gui, int id ) {
	return ((hgeGUIButton*)gui->GetCtrl(id))->GetState();
}
inline void hgeButtonSetState( hgeGUI * gui, int id, bool b ) {
	((hgeGUIButton*)gui->GetCtrl(id))->SetState(b);
}
inline float hgeSliderGetValue( hgeGUI * gui, int id) {
	return ((hgeGUISlider*)gui->GetCtrl(id))->GetValue();
}
inline void hgeSliderSetValue( hgeGUI * gui, int id, float f ) {
	((hgeGUISlider*)gui->GetCtrl(id))->SetValue(f);
}
inline hgeGUIText * hgeGetTextCtrl( hgeGUI * gui, int id ) {
	return ((hgeGUIText*)gui->GetCtrl(id));
}
inline hgeGUIListbox * hgeGetListboxCtrl( hgeGUI * gui, int id) {
	return ((hgeGUIListbox*)gui->GetCtrl(id));
}

} // namespace hge
