/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeGUI helper class implementation
 */

#include <hgegui.h>

namespace hge {

HGE * g_hgegui_hge = nullptr;
HGE * g_hgeguiobject_hge = nullptr;


hgeGUI::hgeGUI()
{
	g_hgegui_hge=hgeCreate(HGE_VERSION);

	m_controls = nullptr;
	m_ctrl_lock = nullptr;
	m_ctrl_focus = nullptr;
	m_ctrl_over = nullptr;
	m_navmode=HGEGUI_NONAVKEYS;
	m_lpressed=m_lreleased=false;
	m_rpressed=m_rreleased=false;
	m_wheel=0;
	m_mouse_x=m_mouse_y=0.0f;
	m_enter_leave=0;
	m_cursor_sprite = nullptr;
}

hgeGUI::~hgeGUI()
{
	hgeGUIObject *ctrl=m_controls, *nextctrl;

	while(ctrl)
	{
		nextctrl=ctrl->next;
		delete ctrl;
		ctrl=nextctrl;
	}

	g_hgegui_hge->Release();
}

void hgeGUI::AddCtrl(hgeGUIObject *ctrl)
{
	hgeGUIObject *last=m_controls;

	ctrl->gui=this;

	if(!m_controls)
	{
		m_controls=ctrl;
		ctrl->prev = nullptr;
		ctrl->next = nullptr;
	}
	else
	{
		while(last->next) last=last->next;
		last->next=ctrl;
		ctrl->prev=last;
		ctrl->next = nullptr;
	}
}

void hgeGUI::DelCtrl(int id)
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		if(ctrl->id == id)
		{
			if(ctrl->prev) ctrl->prev->next = ctrl->next;
			else m_controls = ctrl->next;
			if(ctrl->next) ctrl->next->prev = ctrl->prev;
			delete ctrl;
			return;
		}
		ctrl=ctrl->next;
	}
}

hgeGUIObject* hgeGUI::GetCtrl(int id) const
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		if(ctrl->id == id) return ctrl;
		ctrl=ctrl->next;
	}

	return NULL;
}

void hgeGUI::MoveCtrl(int id, float x, float y)
{
	hgeGUIObject *ctrl=GetCtrl(id);
	ctrl->rect.x2=x + (ctrl->rect.x2 - ctrl->rect.x1);
	ctrl->rect.y2=y + (ctrl->rect.y2 - ctrl->rect.y1);
	ctrl->rect.x1=x;
	ctrl->rect.y1=y;
}

void hgeGUI::ShowCtrl(int id, bool bVisible)
{
	GetCtrl(id)->bVisible=bVisible;
}

void hgeGUI::EnableCtrl(int id, bool bEnabled)
{
	GetCtrl(id)->bEnabled=bEnabled;
}

void hgeGUI::SetNavMode(int mode)
{
	m_navmode=mode;
}

void hgeGUI::SetCursor(hgeSprite *spr)
{
	m_cursor_sprite=spr;
}


void hgeGUI::SetColor(uint32_t color)
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		ctrl->SetColor(color);
		ctrl=ctrl->next;
	}
}


void hgeGUI::Reset()
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		ctrl->Reset();
		ctrl=ctrl->next;
	}

	m_ctrl_lock = nullptr;
	m_ctrl_over = nullptr;
	m_ctrl_focus = nullptr;
}


void hgeGUI::Move(float dx, float dy)
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		ctrl->rect.x1 += dx;
		ctrl->rect.y1 += dy;
		ctrl->rect.x2 += dx;
		ctrl->rect.y2 += dy;

		ctrl=ctrl->next;
	}
}


void hgeGUI::SetFocus(int id)
{
	hgeGUIObject *ctrlNewFocus=GetCtrl(id);

	if(ctrlNewFocus==m_ctrl_focus) return;
	if(!ctrlNewFocus)
	{
		if(m_ctrl_focus) m_ctrl_focus->Focus(false);
		m_ctrl_focus = nullptr;
	}
	else if(!ctrlNewFocus->bStatic && ctrlNewFocus->bVisible && ctrlNewFocus->bEnabled)
	{
		if(m_ctrl_focus) m_ctrl_focus->Focus(false);
		if(ctrlNewFocus) ctrlNewFocus->Focus(true);
		m_ctrl_focus=ctrlNewFocus;
	}
}

int hgeGUI::GetFocus() const
{
	if(m_ctrl_focus) return m_ctrl_focus->id;
	else return 0;
}

void hgeGUI::Enter()
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		ctrl->Enter();
		ctrl=ctrl->next;
	}

	m_enter_leave=2;
}

void hgeGUI::Leave()
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		ctrl->Leave();
		ctrl=ctrl->next;
	}

	m_ctrl_focus = nullptr;
	m_ctrl_over = nullptr;
	m_ctrl_lock = nullptr;
	m_enter_leave=1;
}

void hgeGUI::Render()
{
	hgeGUIObject *ctrl=m_controls;

	while(ctrl)
	{
		if(ctrl->bVisible) ctrl->Render();
		ctrl=ctrl->next;
	}

	if(g_hgegui_hge->Input_IsMouseOver() && m_cursor_sprite) m_cursor_sprite->Render(m_mouse_x,m_mouse_y);
}

int hgeGUI::Update(float dt)
{
	bool bDone;
	int key;
	hgeGUIObject *ctrl;

// Update the mouse variables

	g_hgegui_hge->Input_GetMousePos(&m_mouse_x, &m_mouse_y);
	m_lpressed  = g_hgegui_hge->Input_KeyDown(HGEK_LBUTTON);
	m_lreleased = g_hgegui_hge->Input_KeyUp(HGEK_LBUTTON);
	m_rpressed  = g_hgegui_hge->Input_KeyDown(HGEK_RBUTTON);
	m_rreleased = g_hgegui_hge->Input_KeyUp(HGEK_RBUTTON);
	m_wheel=g_hgegui_hge->Input_GetMouseWheel();

// Update all controls

	ctrl=m_controls;
	while(ctrl)
	{
		ctrl->Update(dt);
		ctrl=ctrl->next;
	}

// Handle Enter/Leave

	if(m_enter_leave)
	{
		ctrl=m_controls; bDone=true;
		while(ctrl)
		{
			if(!ctrl->IsDone()) { bDone=false; break; }
			ctrl=ctrl->next;
		}
		if(!bDone) return 0;
		else
		{
			if(m_enter_leave==1) return -1;
			else m_enter_leave=0;
		}
	}

// Handle keys	

	key=g_hgegui_hge->Input_GetKey();
	if(((m_navmode & HGEGUI_LEFTRIGHT) && key==HGEK_LEFT) ||
		((m_navmode & HGEGUI_UPDOWN) && key==HGEK_UP))
	{
		ctrl=m_ctrl_focus;
		if(!ctrl)
		{
			ctrl=m_controls;
			if(!ctrl) return 0;
		}
		do {
			ctrl=ctrl->prev;
			if(!ctrl && ((m_navmode & HGEGUI_CYCLED) || !m_ctrl_focus))
			{
				ctrl=m_controls;
				while(ctrl->next) ctrl=ctrl->next;
			}
			if(!ctrl || ctrl==m_ctrl_focus) break;
		} while(ctrl->bStatic==true || ctrl->bVisible==false || ctrl->bEnabled==false);

		if(ctrl && ctrl!=m_ctrl_focus)
		{
			if(m_ctrl_focus) m_ctrl_focus->Focus(false);
			if(ctrl) ctrl->Focus(true);
			m_ctrl_focus=ctrl;
		}
	}
	else if(((m_navmode & HGEGUI_LEFTRIGHT) && key==HGEK_RIGHT) ||
		((m_navmode & HGEGUI_UPDOWN) && key==HGEK_DOWN))
	{
		ctrl=m_ctrl_focus;
		if(!ctrl)
		{
			ctrl=m_controls;
			if(!ctrl) return 0;
			while(ctrl->next) ctrl=ctrl->next;
		}
		do {
			ctrl=ctrl->next;
			if(!ctrl && ((m_navmode & HGEGUI_CYCLED) || !m_ctrl_focus)) ctrl=m_controls;
			if(!ctrl || ctrl==m_ctrl_focus) break;
		} while(ctrl->bStatic==true || ctrl->bVisible==false || ctrl->bEnabled==false);

		if(ctrl && ctrl!=m_ctrl_focus)
		{
			if(m_ctrl_focus) m_ctrl_focus->Focus(false);
			if(ctrl) ctrl->Focus(true);
			m_ctrl_focus=ctrl;
		}
	}
	else if(m_ctrl_focus && key && key!=HGEK_LBUTTON && key!=HGEK_RBUTTON)
	{
		if(m_ctrl_focus->KeyClick(key, g_hgegui_hge->Input_GetChar())) return m_ctrl_focus->id;
	}

// Handle mouse

	bool bLDown = g_hgegui_hge->Input_GetKeyState(HGEK_LBUTTON);
	bool bRDown = g_hgegui_hge->Input_GetKeyState(HGEK_RBUTTON);

	if(m_ctrl_lock)
	{
		ctrl=m_ctrl_lock;
		if(!bLDown && !bRDown) m_ctrl_lock = nullptr;
		if(ProcessCtrl(ctrl)) return ctrl->id;
	}
	else
	{
		// Find last (topmost) control

		ctrl=m_controls;
		if(ctrl)
			while(ctrl->next) ctrl=ctrl->next;

		while(ctrl)
		{
			if(ctrl->rect.TestPoint(m_mouse_x,m_mouse_y) && ctrl->bEnabled)
			{
				if(m_ctrl_over != ctrl)
				{
					if(m_ctrl_over) m_ctrl_over->MouseOver(false);
					ctrl->MouseOver(true);
					m_ctrl_over=ctrl;
				}

				if(ProcessCtrl(ctrl)) return ctrl->id;
				else return 0;
			}
			ctrl=ctrl->prev;
		}

		if(m_ctrl_over) {m_ctrl_over->MouseOver(false); m_ctrl_over = nullptr;}

	}

	return 0;
}

bool hgeGUI::ProcessCtrl(hgeGUIObject *ctrl)
{
	bool bResult=false;

	if(m_lpressed)	{ m_ctrl_lock=ctrl;SetFocus(ctrl->id);bResult=bResult || ctrl->MouseLButton(true); }
	if(m_rpressed)	{ m_ctrl_lock=ctrl;SetFocus(ctrl->id);bResult=bResult || ctrl->MouseRButton(true); }
	if(m_lreleased)	{ bResult=bResult || ctrl->MouseLButton(false); }
	if(m_rreleased)	{ bResult=bResult || ctrl->MouseRButton(false); }
	if(m_wheel)		{ bResult=bResult || ctrl->MouseWheel(m_wheel); }
	bResult=bResult || ctrl->MouseMove(m_mouse_x-ctrl->rect.x1,m_mouse_y-ctrl->rect.y1);

	return bResult;
}

HGE * hgeGUI::get_hge()
{
	return g_hgegui_hge;
}


HGE * hgeGUIObject::get_hge()
{
	return g_hgeguiobject_hge;
}

hgeGUIObject::hgeGUIObject()
{
	g_hgeguiobject_hge=hgeCreate(HGE_VERSION); color=0xFFFFFFFF;
}

hgeGUIObject::hgeGUIObject( const hgeGUIObject &go )
{

}

hgeGUIObject::~hgeGUIObject()
{
	g_hgeguiobject_hge->Release();
}

hgeGUIObject& hgeGUIObject::operator=( const hgeGUIObject &go )
{
	return *this;
}

} // namespace hge
