/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 *
 * hgeGUI helper classes header
 */
#pragma once

#include <hge.h>
#include <hgesprite.h>
#include <hgerect.h>

namespace hge {

enum {
	HGEGUI_NONAVKEYS	= 0,
	HGEGUI_LEFTRIGHT	= 1,
	HGEGUI_UPDOWN		= 2,
	HGEGUI_CYCLED		= 4
};

class hgeGUI;

/*
** hgeGUIObject
*/
class HGE_EXPORT hgeGUIObject
{
public:
	hgeGUIObject();
	virtual			~hgeGUIObject();

	virtual void	Render() = 0;
	virtual void	Update(float dt) {}

	virtual void	Enter() {}
	virtual void	Leave() {}
	virtual void	Reset() {}
	virtual bool	IsDone() { return true; }
	virtual void	Focus(bool bFocused) {}
	virtual void	MouseOver(bool bOver) {}

	virtual bool	MouseMove(float x, float y) { return false; }
	virtual bool	MouseLButton(bool bDown) { return false; }
	virtual bool	MouseRButton(bool bDown) { return false; }
	virtual bool	MouseWheel(int nNotches) { return false; }
	virtual bool	KeyClick(int key, int chr) { return false; }

	virtual void	SetColor(uint32_t _color) { color=_color; }
	
	int				id;
	bool			bStatic;
	bool			bVisible;
	bool			bEnabled;
	hgeRect			rect;
	uint32_t		color;

	hgeGUI			*gui;
	hgeGUIObject	*next;
	hgeGUIObject	*prev;

protected:
	hgeGUIObject(const hgeGUIObject &go);
	hgeGUIObject&	operator= (const hgeGUIObject &go);

	//static HGE		*g_hgeguiobject_hge;
	static HGE * get_hge();
};


/*
** hgeGUI
*/
class HGE_EXPORT hgeGUI
{
public:
	hgeGUI();
	~hgeGUI();

	void			AddCtrl(hgeGUIObject *ctrl);
	void			DelCtrl(int id);
	hgeGUIObject*	GetCtrl(int id) const;

	void			MoveCtrl(int id, float x, float y);
	void			ShowCtrl(int id, bool bVisible);
	void			EnableCtrl(int id, bool bEnabled);

	void			SetNavMode(int mode);
	void			SetCursor(hgeSprite *spr);
	void			SetColor(uint32_t color);
	void			SetFocus(int id);
	int				GetFocus() const;
	
	void			Enter();
	void			Leave();
	void			Reset();
	void			Move(float dx, float dy);

	int				Update(float dt);
	void			Render();

private:
	hgeGUI(const hgeGUI &);
	hgeGUI&			operator= (const hgeGUI&);
	bool			ProcessCtrl(hgeGUIObject *ctrl);

	//static HGE		*g_hgegui_hge;
	static HGE * get_hge();

	hgeGUIObject	* m_controls;
	hgeGUIObject	* m_ctrl_lock;
	hgeGUIObject	* m_ctrl_focus;
	hgeGUIObject	* m_ctrl_over;

	int				m_navmode;
	int				m_enter_leave;
	hgeSprite		* m_cursor_sprite;

	float			m_mouse_x;
	float			m_mouse_y;
	int				m_wheel;
	bool			m_lpressed, m_lreleased;
	bool			m_rpressed, m_rreleased;
};


} // namespace hge
