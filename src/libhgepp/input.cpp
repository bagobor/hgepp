/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: input
 */
#include "hge_impl.h"

namespace hge {

char *KeyNames[] =
{
 "?",
 "Left Mouse Button", "Right Mouse Button", "?", "Middle Mouse Button",
 "?", "?", "?", "Backspace", "Tab", "?", "?", "?", "Enter", "?", "?",
 "Shift", "Ctrl", "Alt", "Pause", "Caps Lock", "?", "?", "?", "?", "?", "?",
 "Escape", "?", "?", "?", "?",
 "Space", "Page Up", "Page Down", "End", "Home",
 "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow",
 "?", "?", "?", "?", "Insert", "Delete", "?",
 "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
 "?", "?", "?", "?", "?", "?", "?",
 "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
 "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
 "Left Win", "Right Win", "Application", "?", "?",
 "NumPad 0", "NumPad 1", "NumPad 2", "NumPad 3", "NumPad 4",
 "NumPad 5", "NumPad 6", "NumPad 7", "NumPad 8", "NumPad 9",
 "Multiply", "Add", "?", "Subtract", "Decimal", "Divide",
 "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "Num Lock", "Scroll Lock",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "Semicolon", "Equals", "Comma", "Minus", "Period", "Slash", "Grave",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?",
 "Left bracket", "Backslash", "Right bracket", "Apostrophe",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?"
};


bool HGE_CALL HGE_Impl::Input_GetEvent(hgeInputEvent *event)
{
	CInputEventList *eptr;

	if(m_input_queue)
	{
		eptr=m_input_queue;
		memcpy(event, &eptr->event, sizeof(hgeInputEvent));
		m_input_queue=eptr->next;
		delete eptr;
		return true;
	}
	
	return false;
}

void HGE_CALL HGE_Impl::Input_GetMousePos(float *x, float *y)
{
	*x=m_xpos; *y=m_ypos;
}


void HGE_CALL HGE_Impl::Input_SetMousePos(float x, float y)
{
	POINT pt;
	pt.x=(long)x; pt.y=(long)y;
	ClientToScreen(m_hwnd, &pt);
	SetCursorPos(pt.x,pt.y);
}

int HGE_CALL HGE_Impl::Input_GetMouseWheel()
{
	return m_zpos;
}

bool HGE_CALL HGE_Impl::Input_IsMouseOver()
{
	return m_mouse_over;
}

bool HGE_CALL HGE_Impl::Input_GetKeyState(int key)
{
	return ((GetKeyState(key) & 0x8000) != 0);
}

bool HGE_CALL HGE_Impl::Input_KeyDown(int key)
{
	return (m_key_states[key] & 1) != 0;
}

bool HGE_CALL HGE_Impl::Input_KeyUp(int key)
{
	return (m_key_states[key] & 2) != 0;
}

char* HGE_CALL HGE_Impl::Input_GetKeyName(int key)
{
	return KeyNames[key];
}

int HGE_CALL HGE_Impl::Input_GetKey()
{
	return m_vkey;
}

int HGE_CALL HGE_Impl::Input_GetChar()
{
	return m_char;
}


//////// Implementation ////////


void HGE_Impl::_InputInit()
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hwnd, &pt);
	m_xpos = (float)pt.x;
	m_ypos = (float)pt.y;

	memset(&m_key_states, 0, sizeof(m_key_states));
}

void HGE_Impl::_UpdateMouse()
{
	POINT	pt;
	RECT	rc;

	GetCursorPos(&pt);
	GetClientRect(m_hwnd, &rc);
	MapWindowPoints(m_hwnd, NULL, (LPPOINT)&rc, 2);

	if(m_captured || (PtInRect(&rc, pt) && WindowFromPoint(pt)==m_hwnd))
		m_mouse_over=true;
	else
		m_mouse_over=false;
}

void HGE_Impl::_BuildEvent(int type, int key, int scan, int flags, int x, int y)
{
	CInputEventList *last, *eptr=new CInputEventList;
	unsigned char kbstate[256];
	POINT pt;

	eptr->event.type=type;
	eptr->event.chr=0;
	pt.x=x; pt.y=y;

	GetKeyboardState(kbstate);
	if(type==INPUT_KEYDOWN)
	{
		if((flags & HGEINP_REPEAT) == 0) m_key_states[key] |= 1;
		ToAscii(key, scan, kbstate, (unsigned short *)&eptr->event.chr, 0);
	}
	if(type==INPUT_KEYUP)
	{
		m_key_states[key] |= 2;
		ToAscii(key, scan, kbstate, (unsigned short *)&eptr->event.chr, 0);
	}
	if(type==INPUT_MOUSEWHEEL)
	{
		eptr->event.key=0; eptr->event.wheel=key;
		ScreenToClient(m_hwnd,&pt);
	}
	else { eptr->event.key=key; eptr->event.wheel=0; }

	if(type==INPUT_MBUTTONDOWN)
	{
		m_key_states[key] |= 1;
		SetCapture(m_hwnd);
		m_captured=true;
	}
	if(type==INPUT_MBUTTONUP)
	{
		m_key_states[key] |= 2;
		ReleaseCapture();
		Input_SetMousePos(m_xpos, m_ypos);
		pt.x=(int)m_xpos; pt.y=(int)m_ypos;
		m_captured=false;
	}
	
	if(kbstate[VK_SHIFT] & 0x80) flags|=HGEINP_SHIFT;
	if(kbstate[VK_CONTROL] & 0x80) flags|=HGEINP_CTRL;
	if(kbstate[VK_MENU] & 0x80) flags|=HGEINP_ALT;
	if(kbstate[VK_CAPITAL] & 0x1) flags|=HGEINP_CAPSLOCK;
	if(kbstate[VK_SCROLL] & 0x1) flags|=HGEINP_SCROLLLOCK;
	if(kbstate[VK_NUMLOCK] & 0x1) flags|=HGEINP_NUMLOCK;
	eptr->event.flags=flags;

	if(pt.x==-1) { eptr->event.x=m_xpos;eptr->event.y=m_ypos; }
	else
	{
		if(pt.x<0) pt.x=0;
		if(pt.y<0) pt.y=0;
		if(pt.x>=m_scr_width) pt.x=m_scr_width-1;
		if(pt.y>=m_scr_height) pt.y=m_scr_height-1;

		eptr->event.x=(float)pt.x;
		eptr->event.y=(float)pt.y;
	}

	eptr->next=0; 

	if(!m_input_queue) m_input_queue=eptr;
	else
	{
		last=m_input_queue;
		while(last->next) last=last->next;
		last->next=eptr;
	}

	if(eptr->event.type==INPUT_KEYDOWN || eptr->event.type==INPUT_MBUTTONDOWN)
	{
		m_vkey=eptr->event.key;m_char=eptr->event.chr;
	}
	else if(eptr->event.type==INPUT_MOUSEMOVE)
	{
		m_xpos=eptr->event.x;m_ypos=eptr->event.y;
	}
	else if(eptr->event.type==INPUT_MOUSEWHEEL)
	{
		m_zpos+=eptr->event.wheel;
	}
}

void HGE_Impl::_ClearQueue()
{
	CInputEventList *nexteptr, *eptr=m_input_queue;

	memset(&m_key_states, 0, sizeof(m_key_states));
	
	while(eptr)
	{
		nexteptr=eptr->next;
		delete eptr;
		eptr=nexteptr;
	}

	m_input_queue=0; m_vkey=0; m_char=0; m_zpos=0;
}

} // namespace hge
